import os
import subprocess
import logging
import inspect

# Configure logging
logging.basicConfig(level=logging.DEBUG, format='%(message)s')

# Function to log messages with extra context (script name, function name, line number)
def log(message, level="info"):
    # Get current frame info for logging the function name and line number
    frame = inspect.currentframe().f_back
    filename = os.path.basename(frame.f_code.co_filename)
    function_name = frame.f_code.co_name
    line_number = frame.f_lineno

    # Prepare the log message with file, function, and line info
    log_message = f"{filename}::{function_name}::{line_number}: {message}"

    # Log based on the specified log level
    if level == "debug":
        logging.debug(log_message)
    elif level == "warning":
        logging.warning(log_message)
    elif level == "error":
        logging.error(log_message)
    else:
        logging.info(log_message)

# Define paths
script_dir = os.path.dirname(os.path.realpath(__file__))
output_file = 'build_info.hpp'
output_path = os.path.join(script_dir, '..', 'src', output_file)

# Function to get the current Git hash
def get_git_hash():
    try:
        result = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).strip().decode('utf-8')
        return result
    except subprocess.CalledProcessError:
        return "Unknown"

# Function to read the current hash from the file
def get_current_hash(output_path):
    if not os.path.exists(output_path):
        return None
    try:
        with open(output_path, 'r') as f:
            for line in f:
                # Check for the line that defines the Git hash in the new format
                if line.startswith('const char* GIT_HASH'):
                    # Split by '=' and extract the part after the '=' sign, then strip out ';' and quotes
                    return line.split('=')[1].strip().strip(';').strip().strip('"')
    except FileNotFoundError:
        return None

# Function to write the new hash to the file
def update_hash_file(output_path, git_hash):
    with open(output_path, 'w') as f:
        f.write(f'const char* GIT_HASH = "{git_hash}";\n')
    log(f'Updated {output_file} with hash: {git_hash}', level="info")

def main():
    # Get the current Git hash
    git_hash = get_git_hash()
    log(f'{git_hash} is the hash from git', level="info")

    # Get the current hash from the file
    current_hash = get_current_hash(output_path)
    if current_hash:
        log(f'{current_hash} is the hash from the header', level="info")
    else:
        log(f'{output_path} does not exist', level="info")

    # If the hashes match, no need to update the file
    if current_hash == git_hash:
        log(f'hashes match, skipping generation of {output_file}', level="info")
    else:
        # Update the file with the new hash
        update_hash_file(output_path, git_hash)

if __name__ == "__main__":
    exit()
    main()
