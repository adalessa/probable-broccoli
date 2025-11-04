####
# Credits: https://github.com/anurag3301
###

import json
import subprocess
from pathlib import Path
import shlex

# Constants
PROJECT_ROOT = Path.cwd()
SRC_FILE = PROJECT_ROOT / "src" / "main.cpp"
CCDB_ORIG = PROJECT_ROOT / "compile_commands.json"
CCDB_BAK = PROJECT_ROOT / "compile_commands.json.bak"
CCLS_FILE = PROJECT_ROOT / ".ccls"
CCDB_NEW = PROJECT_ROOT / "compile_commands.json"

KEEP_FLAGS = ("-I", "-D", "-std", "-Wall")


def ensure_compile_commands():
    if not CCDB_ORIG.exists():
        subprocess.run(["pio", "run", "-t", "compiledb"], check=True)
    if not CCDB_BAK.exists():
        CCDB_ORIG.rename(CCDB_BAK)


def extract_compiler():
    return "g++"


def parse_ccls():
    common_flags = []
    cpp_flags = []

    with open(CCLS_FILE, "r", encoding="utf-8") as f:
        for raw_line in f:
            line = raw_line.strip()
            if not line or line.startswith("#"):
                continue

            if line.startswith("%cpp"):
                target = cpp_flags
                line = line[len("%cpp"):].strip()
            elif line.startswith("%c"):
                continue
            elif line.startswith("%"):
                continue
            else:
                target = common_flags

            # Manual parse to handle spaces in include paths
            i = 0
            while i < len(line):
                # Find next occurrence of -I or -D
                if line[i:i+2] in ("-I", "-D"):
                    prefix = line[i:i+2]
                    i += 2
                    start = i

                    # Read until next " -" (space + dash) that actually starts a flag,
                    # or until end of line. Use bounds-safe checks.
                    while i < len(line):
                        # If current char is space and next char exists and is '-',
                        # assume this begins the next flag -> stop before this space.
                        if line[i] == " " and (i + 1) < len(line) and line[i+1] == "-":
                            break
                        i += 1

                    # Now take from start..i (i is either index of the space before next flag,
                    # or len(line) if we reached the end) — include all characters.
                    value = line[start:i].strip()
                    full_flag = f"{prefix}{value}"
                    target.append(full_flag)

                    # skip any spaces before continuing
                    while i < len(line) and line[i] == " ":
                        i += 1
                else:
                    i += 1

    # Filter flags to only those we want
    filtered = [
        flag for flag in (common_flags + cpp_flags)
        if flag == "-Wall" or flag.startswith(KEEP_FLAGS)
    ]
    return filtered


def generate_single_entry_compile_command(compiler, flags):
    # Quote flags with shlex.quote so paths with spaces are a single shell token
    quoted_flags = [shlex.quote(f) for f in flags]
    cmd = " ".join([compiler] + quoted_flags + [str(SRC_FILE)])
    entry = {
        "directory": str(PROJECT_ROOT),
        "file": str(SRC_FILE),
        "command": cmd,
    }
    CCDB_NEW.write_text(json.dumps([entry], indent=2))
    return 1


# Execution
ensure_compile_commands()
compiler_path = extract_compiler()
filtered_flags = parse_ccls()
entry_count = generate_single_entry_compile_command(compiler_path, filtered_flags)
