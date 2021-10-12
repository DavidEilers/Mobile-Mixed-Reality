package com.example.teampraktikum;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.hardware.display.DisplayManager;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import com.google.android.material.snackbar.Snackbar;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.res.AssetManager;


public class ARCoreActivity extends AppCompatActivity implements GLSurfaceView.Renderer, DisplayManager.DisplayListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("full-lib");
        //System.loadLibrary("glbuffer");
    }

    private GLSurfaceView surfaceView;
    private GestureDetector gestureDetector;
    private int viewPortWidth;
    private int viewPortHeight;
    private boolean viewPortChanged;
    private static final String CAMERA_PERMISSION = Manifest.permission.CAMERA;
    private static final int CAMERA_PERMISSION_CODE = 0;
    private double prevTimeInMillis;


    //This SnackbarCode is from Hello_AR_C example project
    private static final int SNACKBAR_UPDATE_INTERVAL_MILLIS = 1000;
    private Snackbar loadingMessageSnackbar;
    private Handler planeStatusCheckingHandler;
    private final Runnable planeStatusCheckingRunnable =
            new Runnable() {
                @Override
                public void run() {
                    // The runnable is executed on main UI thread.
                    try {
                        if (hasDetectedSurface()) {
                            if (loadingMessageSnackbar != null) {
                                loadingMessageSnackbar.dismiss();
                            }
                            loadingMessageSnackbar = null;
                        } else {
                            planeStatusCheckingHandler.postDelayed(
                                    planeStatusCheckingRunnable, SNACKBAR_UPDATE_INTERVAL_MILLIS);
                        }
                    } catch (Exception e) {
                        System.out.println(e.getMessage());
                    }
                }
            };

    public int getStatusBarHeight() {
        int result = 0;
        int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = getResources().getDimensionPixelSize(resourceId);
        }
        resourceId = getResources().getIdentifier("title_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result += getResources().getDimensionPixelSize(resourceId);
        }

        resourceId = getResources().getIdentifier("action_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result += getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_arcore);
        surfaceView = (GLSurfaceView) findViewById(R.id.surfaceview);
        // Set up renderer.
        surfaceView.setPreserveEGLContextOnPause(true);
        surfaceView.setEGLContextClientVersion(2);
        surfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0); // Alpha used for plane blending.
        surfaceView.setRenderer(this);
        surfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        surfaceView.setWillNotDraw(false);


        // Set up touch listener.
        gestureDetector =
                new GestureDetector(
                        this,
                        new GestureDetector.SimpleOnGestureListener() {
                            @Override
                            public boolean onSingleTapUp(final MotionEvent e) {
                                // For devices that support the Depth API, shows a dialog to suggest enabling
                                // depth-based occlusion. This dialog needs to be spawned on the UI thread.
                                //System.out.println("Statusbar height; " + getStatusBarHeight());
                                surfaceView.queueEvent(
                                        () -> nativeOnTouched(e.getX(), e.getY()));
                                return true;
                            }

                            @Override
                            public boolean onDown(MotionEvent e) {
                                return true;
                            }
                        });

        surfaceView.setOnTouchListener(
                (View v, MotionEvent event) -> gestureDetector.onTouchEvent(event));

        int offset[] = new int[2];
        surfaceView.getLocationOnScreen(offset);
        int height = surfaceView.getHeight();
        int width = surfaceView.getWidth();
        //nativeSetCanvasOffset(offset[0], offset[1]);
        System.out.println("OFFSET  x: " + offset[0] + " y: " + offset[1]);

        //JniInterface.assetManager = getAssets();
        // Example of a call to a native method
        //TextView tv = findViewById(R.id.sample_text);
        //tv.setText(stringFromJNI(getApplicationContext()));
        planeStatusCheckingHandler = new Handler();
        prevTimeInMillis=System.currentTimeMillis();
    }

    @Override
    protected void onResume() {
        super.onResume();
        System.out.println("OnResume");
        if (ContextCompat.checkSelfPermission(this, CAMERA_PERMISSION)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(
                    this, new String[]{CAMERA_PERMISSION}, CAMERA_PERMISSION_CODE);
            return;
        }
        System.out.println("Camera_Permission checked");
        if (nativeOnResume(getApplicationContext()) != 0) {
            System.out.println("NativeResume not finished");
            return;
        }
        System.out.println("NativeOnResume finished");
        surfaceView.onResume();
        loadingMessageSnackbar =
                Snackbar.make(
                        ARCoreActivity.this.findViewById(android.R.id.content),
                        "Searching for surfaces...",
                        Snackbar.LENGTH_INDEFINITE);
        // Set the snackbar background to light transparent black color.
        loadingMessageSnackbar.getView().setBackgroundColor(0xbf323232);
        loadingMessageSnackbar.show();
        planeStatusCheckingHandler.postDelayed(
                planeStatusCheckingRunnable, SNACKBAR_UPDATE_INTERVAL_MILLIS);

        // Listen to display changed events to detect 180° rotation, which does not cause a config
        // change or view resize.
        getSystemService(DisplayManager.class).registerDisplayListener(this, null);
        System.out.println("Finished OnResume");
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            // Standard Android full-screen functionality.
            getWindow()
                    .getDecorView()
                    .setSystemUiVisibility(
                            View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                    | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                    | View.SYSTEM_UI_FLAG_FULLSCREEN
                                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        surfaceView.onPause();
        getSystemService(DisplayManager.class).unregisterDisplayListener(this);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     *
     * @param context
     */

    public native int nativeOnResume(Context context);

    public native void onDisplayGeometryChanged(int display_rotation, int width, int height);

    public native void onDrawFrame();

    public native void nativeOnSurfaceCreated(AssetManager assetManager,long pointer, String gamemode);

    public native void nativeOnTouched(float x, float y);

    public native boolean hasDetectedSurface();
    //public native void nativeSetCanvasOffset(int x, int y);

    public native void startConnection(String ipAddress);

    @Override
    public void onDisplayAdded(int displayId) {

    }

    @Override
    public void onDisplayRemoved(int displayId) {

    }

    @Override
    public void onDisplayChanged(int displayId) {
        viewPortChanged = true;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        GLES20.glClearColor(0.7f, 0.0f, 0.0f, 1.0f);
        Intent intent=getIntent();
        String gameModeStr = intent.getStringExtra("GameMode");
        GameModes gameMode = GameModes.valueOf(gameModeStr);
        if(gameMode==GameModes.TICTACTOE){
            nativeOnSurfaceCreated(getAssets(),intent.getLongExtra("TTTGamePointer",0),gameModeStr);
        }else if(gameMode==GameModes.FOURINAROW){
            nativeOnSurfaceCreated(getAssets(),intent.getLongExtra("FourGamePointer",0),gameModeStr);
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        viewPortWidth = width;
        viewPortHeight = height;
        viewPortChanged = true;
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        double timeNow = System.currentTimeMillis();
        double frameTime = timeNow-prevTimeInMillis;
        //System.out.println("Frametime: "+frameTime+"ms or"+(1000/frameTime)+"fps");
        prevTimeInMillis=timeNow;
        synchronized (this) {
            if (viewPortChanged == true) {
                int displayRotation = getWindowManager().getDefaultDisplay().getRotation();
                onDisplayGeometryChanged(displayRotation, viewPortWidth, viewPortHeight);
                viewPortChanged = false;
            }
            //GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
            onDrawFrame();
        }
    }
}
