param (
    [Parameter(Mandatory=$false)]
    [Switch]$useDebug,
    [Parameter(Mandatory=$false)]
    [Switch]$log
)

& ./build.ps1
if ($useDebug.IsPresent) {
    & adb push build/debug/libquestui.so /sdcard/ModData/com.beatgames.beatsaber/Modloader/libs/libquestui.so
} else {
    & adb push build/libquestui.so /sdcard/ModData/com.beatgames.beatsaber/Modloader/libs/libquestui.so
}

& adb shell am force-stop com.beatgames.beatsaber
& adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
Start-Sleep -Seconds 1.0
& adb shell am start com.beatgames.beatsaber/com.unity3d.player.UnityPlayerActivity
if ($log.IsPresent) {
    & ./log.ps1
}