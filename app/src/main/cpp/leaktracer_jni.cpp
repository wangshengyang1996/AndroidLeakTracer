#include <jni.h>
#include <string>
#include "MapMemoryInfo.hpp"
#include "MemoryTrace.hpp"
#include "Object.h"
#include <fstream>
#include <unwind.h>


/** starts monitoring memory allocations in all threads */
void leaktracer_startMonitoringAllThreads(
        JNIEnv *env,
        jclass) {
    leaktracer::MemoryTrace::GetInstance().startMonitoringAllThreads();
}

/** starts monitoring memory allocations in current thread */
void leaktracer_startMonitoringThisThread(
        JNIEnv *env,
        jclass) {
    leaktracer::MemoryTrace::GetInstance().startMonitoringThisThread();
}

/** stops monitoring memory allocations (in all threads or in
 *   this thread only, depends on the function used to start
 *   monitoring
 */
void leaktracer_stopMonitoringAllocations(
        JNIEnv *env,
        jclass) {
    leaktracer::MemoryTrace::GetInstance().stopMonitoringAllocations();
}

/** stops all monitoring - both of allocations and releases */
void leaktracer_stopAllMonitoring(
        JNIEnv *env,
        jclass) {
    leaktracer::MemoryTrace::GetInstance().stopAllMonitoring();
}

/** writes report with all memory leaks */
void leaktracer_writeLeaksToFile(
        JNIEnv *env,
        jclass,
        jstring filePath
) {
    const char *path = env->GetStringUTFChars(filePath, JNI_FALSE);
    leaktracer::MemoryTrace::GetInstance().writeLeaksToFile(path);
    env->ReleaseStringUTFChars(filePath, path);
}

/**  手动触发内存泄漏 **/
void performLeak(
        JNIEnv *env,
        jobject /* this */) {
    // 申请内存但是不释放
    void *leakedMallocMem = malloc(100);
    // 申请内存并释放
    void *safeMallocMem = malloc(100);
    free(safeMallocMem);

    // 创建对象但是不释放
    Object *leakedObject = new Object();
    // 创建对象并释放
    Object *safeObject = new Object();
    delete (safeObject);
}

int JNI_OnLoad(JavaVM *javaVM, void *reserved) {
    JNIEnv *jniEnv;
    if (JNI_OK == javaVM->GetEnv((void **) (&jniEnv), JNI_VERSION_1_4)) {
        jclass registerClass = jniEnv->FindClass(
                "com/wsy/androidleaktracer/NativeMemoryLeakDetector");
        JNINativeMethod jniNativeMethods[] = {
                //3个参数分别为 Java函数的名称，Java函数的签名（不带函数名），本地函数指针
                {"startMonitoringAllThreads", "()V",                   (void *) (leaktracer_startMonitoringAllThreads)},
                {"startMonitoringThisThread", "()V",                   (void *) (leaktracer_startMonitoringThisThread)},
                {"stopMonitoringAllocations", "()V",                   (void *) (leaktracer_stopMonitoringAllocations)},
                {"stopAllMonitoring",         "()V",                   (void *) (leaktracer_stopAllMonitoring)},
                {"writeLeaksToFile",          "(Ljava/lang/String;)V", (void *) (leaktracer_writeLeaksToFile)},
                {"performLeak",               "()V",                   (void *) (performLeak)}
        };
        if (jniEnv->RegisterNatives(registerClass, jniNativeMethods,
                                    sizeof(jniNativeMethods) / sizeof((jniNativeMethods)[0])) < 0) {
            return JNI_ERR;
        }
    }
    return JNI_VERSION_1_4;
}

