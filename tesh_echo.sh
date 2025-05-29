#!/bin/bash

MINISHELL=./minishell
BASH=/bin/bash

commands=(
	'echo ""$?""'
	'echo $?"42"'
	'echo ''$?''"42"'
	'echo '"$USER"''
	'echo "'$USER'"'
	'echo ""'$USER'""'
	'echo '"'$USER'"''
	'echo "''$USER''"'
	'echo "'"'$USER'"'"'
	'echo '"'"$USER"'"''
	'echo $"HOME"$USER'
	'echo $"HOM"E$USER'
	'echo "exit_code ->$? user ->$USER home -> $HOME"'
	'echo $"HOME"'
	'echo $"42$"'
	'echo "$ "'
	'echo hi >./outfiles/outfile01 | echo bye'
)

for cmd in "${commands[@]}"; do
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
  else
    echo "❌ Test Failed"
  fi

  echo "=========================================="
  echo
done

# Cleanup
rm -f m_out.txt b_out.txt m_err.txt b_err.txt
