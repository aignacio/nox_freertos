
import re
import sys

# Define regular expressions for matching variables and macro definitions
VAR_PATTERN = r"\b[A-Za-z_][A-Za-z0-9_]*\b"
MACRO_PATTERN = r"\#define\s+(" + VAR_PATTERN + r")\s+(.+)"

# Define the source and destination file paths from the command-line arguments
if len(sys.argv) != 4:
    print("Usage: python linker_script.py <header_file> <source_script> <output_script>")
    sys.exit(1)

header_file = sys.argv[1]
source_script = sys.argv[2]
output_script = sys.argv[3]

# Read the header file to extract macro definitions
macro_defs = {}
with open(header_file, "r") as f:
    for line in f:
        match = re.match(MACRO_PATTERN, line)
        if match:
            var_name = match.group(1)
            var_value = match.group(2)
            macro_defs[var_name] = var_value

# Read the source linker script and perform search and replace
with open(source_script, "r") as f:
    linker_script = f.read()

for var_name in macro_defs:
    var_pattern = r"\b" + var_name + r"\b"
    var_value = macro_defs[var_name]
    linker_script = re.sub(var_pattern, var_value, linker_script)

# Write the modified linker script to the output file
with open(output_script, "w") as f:
    f.write(linker_script)
