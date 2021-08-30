param ($p1)

if (-not $p1)
{
    $p1 = 10
}

$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk --output-sync=none -j $p1
