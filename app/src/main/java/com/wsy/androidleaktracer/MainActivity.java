package com.wsy.androidleaktracer;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private static final int ACTION_REQUEST_PERMISSIONS = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(this,new String[]{ Manifest.permission.WRITE_EXTERNAL_STORAGE},ACTION_REQUEST_PERMISSIONS);
        }else {
            testNativeLeak();
        }
    }

    private void testNativeLeak() {
        NativeMemoryLeakDetector.startMonitoringAllThreads();
        NativeMemoryLeakDetector.performLeak();
        NativeMemoryLeakDetector.stopAllMonitoring();
        NativeMemoryLeakDetector.writeLeaksResultToFile("sdcard/leakLog.txt");
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == ACTION_REQUEST_PERMISSIONS) {
            boolean isAllGranted = true;
            for (int grantResult : grantResults) {
                isAllGranted &= (grantResult == PackageManager.PERMISSION_GRANTED);
            }
            if (isAllGranted) {
                testNativeLeak();
            } else {
                Toast.makeText(this.getApplicationContext(),"权限被拒绝", Toast.LENGTH_SHORT).show();
            }
        }
    }
}
