package com.wsy.androidleaktracer;

import java.io.File;

public class NativeMemoryLeakDetector {

    static {
        System.loadLibrary("leaktracer");
    }

    /**
     * starts monitoring memory allocations in all threads
     */
    public static native void startMonitoringAllThreads();

    /**
     * starts monitoring memory allocations in current thread
     */
    public static native void startMonitoringThisThread();

    /**
     * stops monitoring memory allocations (in all threads or in
     * this thread only, depends on the function used to start
     * monitoring
     */
    public static native void stopMonitoringAllocations();

    /**
     * stops all monitoring - both of allocations and releases
     */
    public static native void stopAllMonitoring();


    /**
     * 手动触发内存泄漏
     **/
    public static native void performLeak();

    /**
     * writes report with all memory leaks
     */
    private static native void writeLeaksToFile(String filePath);

    /**
     * writes report with all memory leaks
     */
    public static void writeLeaksResultToFile(String filePath) {
        if (filePath == null) {
            throw new NullPointerException("filePath is null");
        }
        File file = new File(filePath);
        if (file.exists() && file.isDirectory()) {
            throw new IllegalArgumentException("can not write data to a directory");
        }
        if (!file.getParentFile().exists() && !file.getParentFile().mkdirs()) {
            throw new IllegalArgumentException("can not create parent folder for file '" + file.getAbsolutePath() + "'");
        }
        writeLeaksToFile(filePath);
    }
}
