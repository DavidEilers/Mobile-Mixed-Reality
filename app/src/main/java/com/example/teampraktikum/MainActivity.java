package com.example.teampraktikum;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.hardware.display.DisplayManager;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.widget.TextView;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageButton;
import android.widget.PopupMenu;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
//import com.google.android.material.snackbar.Snackbar;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity implements GLSurfaceView.Renderer, DisplayManager.DisplayListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private GLSurfaceView surfaceView;
    private int viewPortWidth;
    private int viewPortHeight;
    private boolean viewPortChanged;
    private static final String CAMERA_PERMISSION = Manifest.permission.CAMERA;
    private static final int CAMERA_PERMISSION_CODE = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        surfaceView = (GLSurfaceView) findViewById(R.id.surfaceview);
        // Set up renderer.
        surfaceView.setPreserveEGLContextOnPause(true);
        surfaceView.setEGLContextClientVersion(2);
        surfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0); // Alpha used for plane blending.
        surfaceView.setRenderer(this);
        surfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        surfaceView.setWillNotDraw(false);

        //JniInterface.assetManager = getAssets();
        // Example of a call to a native method
        //TextView tv = findViewById(R.id.sample_text);
        //tv.setText(stringFromJNI(getApplicationContext()));
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (ContextCompat.checkSelfPermission(this, CAMERA_PERMISSION)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(
                    this, new String[]{CAMERA_PERMISSION}, CAMERA_PERMISSION_CODE);
            return;
        }
        nativeOnResume(getApplicationContext());
        surfaceView.onResume();
//        loadingMessageSnackbar =
//                Snackbar.make(
//                        HelloArActivity.this.findViewById(android.R.id.content),
//                        "Searching for surfaces...",
//                        Snackbar.LENGTH_INDEFINITE);
//        // Set the snackbar background to light transparent black color.
//        loadingMessageSnackbar.getView().setBackgroundColor(0xbf323232);
//        loadingMessageSnackbar.show();
//        planeStatusCheckingHandler.postDelayed(
//                planeStatusCheckingRunnable, SNACKBAR_UPDATE_INTERVAL_MILLIS);

        // Listen to display changed events to detect 180° rotation, which does not cause a config
        // change or view resize.
        getSystemService(DisplayManager.class).registerDisplayListener(this, null);
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
    public native String stringFromJNI(Context context);

    public native void nativeOnResume(Context context);

    public native void onDisplayGeometryChanged(int display_rotation, int width, int height);

    public native void onDrawFrame();

    @Override
    public void onDisplayAdded(int displayId) {

    }

    @Override
    public void onDisplayRemoved(int displayId) {

    }

    @Override
    public void onDisplayChanged(int displayId) {

    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        GLES20.glClearColor(0.7f, 0.0f, 0.0f, 1.0f);

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        viewPortWidth = width;
        viewPortHeight = height;
        viewPortChanged = true;
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        synchronized (this) {
            if (viewPortChanged == true) {
                int displayRotation = getWindowManager().getDefaultDisplay().getRotation();
                onDisplayGeometryChanged(displayRotation, viewPortWidth, viewPortHeight);
                viewPortChanged = false;
            }
            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        }
    }
}
