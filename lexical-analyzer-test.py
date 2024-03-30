#!/usr/bin/env python3
import os
import subprocess
import re

DIR = "build"
FILE = "lexical-analyzer-starter"

RED = "\033[0;31m"
GREEN = "\033[0;32m"
NC = "\033[0m"


def abs_path(*args: str) -> str:
    return os.path.abspath(os.path.join(os.path.curdir, *args))


def prepare():
    if not os.path.exists(DIR):
        os.makedirs(DIR)
    subprocess.run(
        [f"cd ./{DIR} && cmake .. && make {FILE}"],
        shell=True,
        stdout=subprocess.DEVNULL,
    )


def run_and_save(actual_file: str, excepted_file: str):
    file = open(actual_file, "w")
    filename = excepted_file.replace(" ", "\ ")
    subprocess.run(
        [f"./{DIR}/{FILE} {filename} {actual_file}"], shell=True, stdout=file
    )


def read_all(path: str, lam=lambda x: x) -> str:
    text = ""
    with open(path) as cin:
        for line in cin:
            text += lam(line)
    return text


def compare_by_lines(text_actual: str, text_expected: str):
    success = True
    for pair in zip(text_expected.splitlines(), text_actual.splitlines()):
        match = re.match(".*(\.[0-9]+)", pair[0])
        p0 = pair[0]
        if match is not None:
            match = match[1]
            replaced = match.rstrip("0").rstrip(".")
            p0 = pair[0].replace(match, replaced)
        if p0 != pair[1]:
            print(f"expected: {p0}")
            print(f"actual: {pair[1]}")
            success = False
    return success


def passed(i: int):
    print(f"{GREEN}test {i} passed{NC}")


def failed(i: int):
    print(f"{RED}test {i} failed{NC}")


def test():
    for i in range(1, 16):
        excepted_file = abs_path("tests", f"{i}_test.olg")
        actual_file = abs_path(DIR, f"{i}_result")

        run_and_save(actual_file, excepted_file)

        text_actual = read_all(
            actual_file,
            lambda x: x[x.find('"') + 1 : x.rfind('"')].replace("\\n", "\n"),
        )
        text_expected = read_all(excepted_file)

        success = compare_by_lines(text_actual, text_expected)
        if success:
            passed(i)
        else:
            failed(i)


if __name__ == "__main__":
    prepare()
    test()
