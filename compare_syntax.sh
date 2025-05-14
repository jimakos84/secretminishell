#!/bin/bash

# ANSI color codes for formatting
GREEN='\033[0;32m'
RED='\033[0;31m'
BOLD='\033[1m'
RESET='\033[0m'

# List of test commands
commands=("<<<" ">>>" "<>" "><" ">><<" "<<>>" ">|" ">>|" "|>" "|>>" "<|" "<<|" "|<" "|<<")

# Path to your minishell executable
MINISHELL=./minishell

# Temporary files to hold stderr outputs
bash_err=$(mktemp)
mini_err=$(mktemp)

echo -e "${BOLD}Comparing syntax errors between bash and minishell${RESET}"
echo "==========================================================="

for cmd in "${commands[@]}"; do
    # Run the command in bash and capture stderr
    bash -c "$cmd" 2> "$bash_err"

    # Run the command in minishell and capture stderr (ignore stdout)
    echo "$cmd" | $MINISHELL 2> "$mini_err" > /dev/null

    # Read the outputs
    bash_output=$(cat "$bash_err")
    mini_output=$(cat "$mini_err")

    # Compare outputs
    if [[ "$bash_output" == "$mini_output" ]]; then
        status="${GREEN}MATCH${RESET}"
    else
        status="${RED}DIFFER${RESET}"
    fi

    # Print comparison
    echo -e "\n${BOLD}Command:${RESET} '$cmd' => Result: $status"
    echo -e "${BOLD}Bash stderr:${RESET}"
    echo -e "$bash_output"
    echo -e "${BOLD}Minishell stderr:${RESET}"
    echo -e "$mini_output"
done

# Clean up temp files
rm -f "$bash_err" "$mini_err"
