$gcc = "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe"
$objCopy = "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe"
$project = "../FlasherCore"
$output = "../output"
$bin = "$output/bin"

$targetMmcu = @("atmega328p", "atmega8")
$targetFcpu = @(16000000)
$targetBaudRate = @(57600, 38400)

function GetDirectoryName([string] $path) {
    $slashIndex = $path.LastIndexOf('/');
    $slash2Index = $path.LastIndexOf('\');
    if ($slash2Index -gt $slashIndex) {
        $slashIndex = $slash2Index;
    }
    if ($slashIndex -ge 0) {
        return $path.Substring(0, $slashIndex);
    }
    return $path;

}

Remove-Item $output -Recurse
New-Item $output -Type Directory | Out-Null
New-Item $bin -Type Directory | Out-Null
foreach ($mmcu in $targetMmcu) {
    foreach ($fcpu in $targetFcpu) {
        foreach ($baudRate in $targetBaudRate) {
            Get-ChildItem "$project/**/*.c" -Recurse | Foreach-Object {
                $root = Get-Item $project
                $rel = $_.FullName.Substring($root.FullName.Length + 1)
                $objectFile = $rel.Replace(".c", ".o");
                $objectFile = "$bin/$objectFile"
                $objectDir = GetDirectoryName($objectFile)
                if (!(Test-Path $objectDir)) {
                    New-Item $objectDir -Type Directory | Out-Null
                }
                & $gcc -x c -funsigned-char -funsigned-bitfields -DDEBUG -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -UF_CPU -USTK_BAUD_RATE -DF_CPU="$fcpu" -DSTK_BAUD_RATE="$baudRate" -mmcu="$mmcu" -c "$project/$rel" -std=gnu99 -o "$objectFile"
            }

            $objects = @();
            Get-ChildItem "$bin/**/*.o" -Recurse | Foreach-Object {
                $objects += $_.FullName
            }
            & $gcc -Wall -Os -UF_CPU -DF_CPU="$fcpu" -mmcu="$mmcu" -o "$bin/FlasherCore.elf" $objects
            & $objCopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "$bin/FlasherCore.elf" "$output/FlasherCore__$($mmcu)_$($fcpu/1000000)MHz_$baudRate.hex"
        }
    }
}
Remove-Item $bin -Recurse
