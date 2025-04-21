#!/bin/bash

EXEC="./minishell"
TEST_ID=1
NL=$'\n'

GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"

print_result() {
  local status="$1"
  local description="$2"
  if [ "$status" == "OK" ]; then
    echo -e "${GREEN}✅ Test $TEST_ID: $description${RESET}"
  else
    echo -e "${RED}❌ Test $TEST_ID: $description${RESET}"
    echo -e "   Minishell: ${RED}$3 (exit $5)${RESET}"
    echo -e "   Bash     : ${GREEN}$4 (exit $6)${RESET}"
  fi
  ((TEST_ID++))
}

run_test() {
  local input="$1"
  local description="$2"

  echo -n "$input" | $EXEC 2> ms_err.txt > ms_out.txt
  ms_exit=$?
  ms_err=$(<ms_err.txt)

  echo -n "$input" | bash 2> bash_err.txt > bash_out.txt
  bash_exit=$?
  bash_err=$(<bash_err.txt)

  if [[ "$ms_exit" == "$bash_exit" && "$ms_err" == "$bash_err" ]]; then
    print_result "OK" "$description"
  else
    print_result "FAIL" "$description" "$ms_err" "$bash_err" "$ms_exit" "$bash_exit"
  fi

  rm -f ms_* bash_*
}

echo "--- Command Parsing Tests ---"

#Basic input tests / Syntax validation tests
#test 01
run_test ""						"Empty input"
#test 02
run_test "      "				"Only spaces"
#test 03
run_test "echo Hello"			"Normal command"
#test 04
run_test "echo 'quoted text'" 	"Single quotes"
#test 05
run_test "echo \"quoted text\""	"Double quotes"
#test 06
run_test "echo \"Unclosed"		"Unclosed double quote"
#test 07
run_test "echo 'Unclosed"		"Unclosed single quote"
#test 08
run_test "echo \"\$USER\""		"Double quote with \$ expansion"
#test 09
run_test "echo '\$USER'"		"Single quote with literal \$"
#test 10
run_test "echo"					"Quoted command"
#test 11
run_test "ech o"				"Invalid command"
#test 12
run_test "pwd"					"Simple command {pwd}"
#test 13
run_test "ls"					"Simple command {ls}"
#test 14
run_test "date"					"Simple command {date}"
#test 15
run_test "whoami"				"Simple command {whoami}"
#test 16
run_test "uname"				"Simple command {uname}"
#test 17
run_test "ls -la"				"Simple command {ls -la}"
#test 18
run_test "ls -l -a"				"Simple command {ls -l -a}"
#test 19
run_test "ls \"\""				"Simple command {ls ""}"
#test 20
run_test "ls \"  \""			"Simple command {ls "  "}"

echo "✅ All parsing tests complete!"
