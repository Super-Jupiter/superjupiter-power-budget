#!/usr/bin/env python3
"""Compile a clean PlatformIO application using an archive or registry coordinate."""
import argparse
from pathlib import Path
import subprocess
import sys
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--package', required=True)
    args = parser.parse_args()
    package = args.package
    if Path(package).is_file():
        package = Path(package).resolve().as_posix()
    with tempfile.TemporaryDirectory(prefix='superjupiter-platformio-consumer-') as temporary:
        project = Path(temporary)
        (project / 'src').mkdir()
        (project / 'platformio.ini').write_text(
            '[env:consumer]\nplatform = espressif32@6.12.0\nboard = esp32dev\n'
            'framework = arduino\nlib_deps = ' + package + '\n', encoding='utf-8')
        (project / 'src/main.cpp').write_text('#include <Arduino.h>\n#include <SuperJupiterPowerBudget.h>\nusing namespace superjupiter::power_budget;\nvoid setup() {\n Serial.begin(115200);\nState profile[]={{80,100},{0.1,9900}};\nResult battery=estimate(profile,2,2000,0.8);\n(void)battery;\n Serial.println("SuperJupiter PowerBudget example ran. Hardware validation pending.");\n}\nvoid loop() {}\n', encoding='utf-8')
        subprocess.run([sys.executable, '-m', 'platformio', 'run', '-d', str(project)], check=True)
    print('PASS: clean PlatformIO package consumer (compilation only)')


if __name__ == '__main__':
    main()
