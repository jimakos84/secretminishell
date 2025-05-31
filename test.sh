#!/bin/bash

MINISHELL=./minishell
BASH=/bin/bash

# Check if a file is passed
if [ $# -ne 1 ]; then
  echo "Usage: $0 <command_file>"
  exit 1
fi

COMMAND_FILE="$1"

if [ ! -f "$COMMAND_FILE" ]; then
  echo "❌ File not found: $COMMAND_FILE"
  exit 1
fi

total=0
passed=0
failed=0
failed_commands=()

while IFS= read -r cmd || [ -n "$cmd" ]; do
  ((total++))
  echo "=========================================="
  echo "🔧 Testing command: $cmd"
  echo

  # Run in minishell
  echo "$cmd" | $MINISHELL > m_out.txt 2> m_err.txt
  m_status=$?

  # Run in bash
  echo "$cmd" | $BASH > b_out.txt 2> b_err.txt
  b_status=$?

  # Read outputs
  m_output=$(cat m_out.txt)
  b_output=$(cat b_out.txt)

  # Display both outputs
  echo "🟦 Minishell STDOUT:"
  echo "$m_output"
  echo "🔚 Minishell Exit Status: $m_status"
  echo

  echo "🟨 Bash STDOUT:"
  echo "$b_output"
  echo "🔚 Bash Exit Status: $b_status"
  echo

  # Compare
  if diff -u m_out.txt b_out.txt > /dev/null && [ "$m_status" -eq "$b_status" ]; then
    echo "✅ Test Passed"
    ((passed++))
  else
    echo "❌ Test Failed"
    ((failed++))
    failed_commands+=("$cmd")
  fi

  echo "=========================================="
  echo
done < "$COMMAND_FILE"

# Cleanup
rm -f m_out.txt b_out.txt m_err.txt b_err.txt

# Summary
echo "============ 🧾 SUMMARY ============"
echo "📦 Total Tests   : $total"
echo "✅ Passed        : $passed"
echo "❌ Failed        : $failed"
echo "===================================="

if [ "$failed" -gt 0 ]; then
  echo
  echo "❗ Failed Commands:"
  for cmd in "${failed_commands[@]}"; do
    echo "🔸 $cmd"
  done
  echo "===================================="
fi
