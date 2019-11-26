import subprocess
import textwrap
from os import listdir
from os import path
from os.path import isfile, join, isdir, exists

tester_ver = 1.2
ASCII_ERROR = """
______  _______  _______  _______  _______ 
(  ____ \(  ____ )(  ____ )(  ___  )(  ____ )
| (    \/| (    )|| (    )|| (   ) || (    )|
| (__    | (____)|| (____)|| |   | || (____)|
|  __)   |     __)|     __)| |   | ||     __)
| (      | (\ (   | (\ (   | |   | || (\ (   
| (____/\| ) \ \__| ) \ \__| (___) || ) \ \__
(_______/|/   \__/|/   \__/(_______)|/   \__/
"""
ASCII_PASSES = """
  , ; ,   .-'^^^'-.   , ; ,
  \\|/  .'         '.  \|//
   \-;-/   ()   ()   \-;-/
   // ;               ; \\
  //__; :.         .; ;__\\
 `-----\'.'-.....-'.'/-----'
        '.'.-.-,_.'.'
          '(  (..-'
            '-'
"""

# paths to files and folder

name_of_good = "good_trees"
name_of_invalid_trees = "invalid_trees"
name_of_no_tree = "no_trees"

path_to_test_files = path.join("tester_files")
path_to_good_trees = path.join(path_to_test_files, name_of_good)
path_to_invalid_trees = path.join(path_to_test_files, name_of_invalid_trees)
path_to_no_trees = path.join(path_to_test_files, name_of_no_tree)

path_to_system_out = path.join(path_to_test_files, "system_out")
path_to_user_out = path.join(path_to_test_files, "user_out")

path_to_c_files1 = path.join("TreeAnalyzer.c")
path_to_c_files2 = path.join("queue.c")

path_to_compiled_files = path.join("ex2.exe")

# name of files
name_of_user_output_file_no_folder = "_user" + "_output" + ".txt"
name_of_user_errors_file_no_folder = "_user" + "_errors" + ".txt"

name_of_school_solution_output_no_folder = "_school_solution" + "_output" + ".txt"
name_of_school_solution_errors_no_folder = "_school_solution" + "_errors" + ".txt"

invalid_file = "invalid.txt"
no_tree_file = "no_tree.txt"
num_of_parm_file = "num_of_parm.txt"

simple_path = path.join(path_to_good_trees, "simple.txt")

invalid = {
    "no_int_num_first": simple_path + " 1.1 2",
    "no_int_num_sec": simple_path + " 1 2.5",
    "char_in_num_1": simple_path + " 2g 4",
    "char_in_num_2": simple_path + " 2 m3",
    "neg_num_1": simple_path + " -2 6",
    "neg_num_2": simple_path + " 2 -6",
    "big_num_1": simple_path + " 8 3",


}


def path_to(name_of_file):
    return path.join(path_to_good_trees, name_of_file)

valid = {
    "simple_1": simple_path + " 4 5",
    "simple_2": simple_path + " 7 2",

    "normal_1_1": path_to("normal_tree_1.txt") + " 1 13",
    "normal_1_2": path_to("normal_tree_1.txt") + " 9 13",
    "normal_1_3": path_to("normal_tree_1.txt") + " 3 3",
    "normal_1_4": path_to("normal_tree_1.txt") + " 11 10",
    "normal_1_5": path_to("normal_tree_1.txt") + " 2 6",

    "long_1": path_to("long.txt") + " 7 10",
    "long_2": path_to("long.txt") + " 8 2",
    "long_3": path_to("long.txt") + " 10 3",
    "long_4": path_to("long.txt") + " 2 10",

    "big_1": path_to("big_tree.txt") + " 2 10",
    "big_2": path_to("big_tree.txt") + " 30 23",
    "big_3": path_to("big_tree.txt") + " 0 19",
    "big_4": path_to("big_tree.txt") + " 15 29",
    "big_5": path_to("big_tree.txt") + " 7 27",

    "one_kodkod" : path_to("one_kodkod.txt") + " 0 0"
}


num_of_parm = {
    "les_parm": simple_path + " 1",
    "more_parm": simple_path + " 1 2 3",
    "no_file": "1 3"
}




def can_not_test(msg):
    print("Error:", msg)
    input("press Enter to exit")
    exit(1)


def run_with_cmd(command_list, str=""):
    """
    Execute the given command list with the command line
    Return a tuple containing the return code, output and errors.
    """

    process = subprocess.Popen(command_list, shell=True, stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                               universal_newlines=True)

    process.communicate(str)
    output, errors = process.communicate()
    return process.returncode, output, errors


def print_with_indentation(title, to_print):
    """print text in a nice way"""
    prefix = title + ": "
    wrapper = textwrap.TextWrapper(initial_indent=prefix,
                                   break_long_words=False,
                                   subsequent_indent=' ' * len(prefix))
    print(wrapper.fill(to_print))


def compile_file():
    """
    compile the java files. the compiled files are located in \test_files\compiled_files.
    terminate the tester if there was an error.
    :return: true if was successful.
    """
    if not exists(path_to_c_files1):
        can_not_test("You don't have the folder: " + path_to_c_files1)

    if not exists(path_to_c_files2):
        can_not_test("You don't have the folder: " + path_to_c_files2)

    command_list = ["gcc", "-Wall", "-Wextra", "-Wvla", "-std=c99", "-lm",
                    path_to_c_files1, path_to_c_files2, "-o", "ex2"]

    code, output, errors = run_with_cmd(command_list)
    print(output, errors)
    if code != 0:
        can_not_test("problem with compiling\n" + "error message\n" + errors)
    print("compile OK\n\n")


def run_one_invalid_test(name_of_test, parm, error):
    name_of_school_solution_output = "empty.txt"
    name_of_school_solution_errors = error

    path_to_school_solution_output = path.join(path_to_system_out,
                                               name_of_school_solution_output)
    path_to_school_solution_errors = path.join(path_to_system_out,
                                               name_of_school_solution_errors)

    name_of_user_output = name_of_test + name_of_user_output_file_no_folder
    name_of_user_errors = name_of_test + name_of_user_errors_file_no_folder

    path_to_user_output = path.join(path_to_user_out, name_of_user_output)
    path_to_user_errors = path.join(path_to_user_out, name_of_user_errors)

    print("starting", name_of_test, "..")

    command_list = [path_to_compiled_files] + parm.split()

    code, user_output, user_errors = run_with_cmd(
        command_list)  # run your code

    # save the files output and errors
    with open(path_to_user_output, 'w') as output_file:
        output_file.write(user_output)

    with open(path_to_user_errors, 'w') as errors_file:
        errors_file.write(user_errors)

    # compare to school solution
    compare_outputs = compare_files(path_to_school_solution_output,
                                    path_to_user_output)
    compare_errors = compare_files(path_to_school_solution_errors,
                                   path_to_user_errors)

    # print helpful information if there was mistakes.
    if compare_outputs is not None or compare_errors is not None:
        # compare failed
        if compare_outputs is not None:
            print("Output file compare failed: here are the details:")
            print_with_indentation("output compare", compare_outputs)
        if compare_errors is not None:
            print("Errors file compare failed: here are the details:")
            print_with_indentation("errors compare", compare_errors)
        return False
    print("passed :)")
    return True


def run_one_good_test(name_of_test, parm):
    """
    run one test. run your code with command file given and with the data given. sva the rusolts in a txt file.
    then compare it to the school solution txt file.
    :param test_folder_name: the name of the folder of the test (tests/test#)
    :param files_to_filter_folder: the data folder  (simple of complex)
    :return: true if was successful.
    """

    name_of_school_solution_output = name_of_test + name_of_school_solution_output_no_folder
    name_of_school_solution_errors = "empty.txt"

    path_to_school_solution_output = path.join(path_to_system_out,
                                               name_of_school_solution_output)
    path_to_school_solution_errors = path.join(path_to_system_out,
                                               name_of_school_solution_errors)

    name_of_user_output = name_of_test + name_of_user_output_file_no_folder
    name_of_user_errors = name_of_test + name_of_user_errors_file_no_folder

    path_to_user_output = path.join(path_to_user_out, name_of_user_output)
    path_to_user_errors = path.join(path_to_user_out, name_of_user_errors)

    print("starting", name_of_test, "..")

    command_list = [path_to_compiled_files] + parm.split()

    code, user_output, user_errors = run_with_cmd(command_list)  # run your code

    # save the files output and errors
    with open(path_to_user_output, 'w') as output_file:
        output_file.write(user_output)

    with open(path_to_user_errors, 'w') as errors_file:
        errors_file.write(user_errors)

    # compare to school solution
    compare_outputs = compare_files(path_to_school_solution_output,
                                    path_to_user_output)
    compare_errors = compare_files(path_to_school_solution_errors,
                                   path_to_user_errors)

    # print helpful information if there was mistakes.
    if compare_outputs is not None or compare_errors is not None:
        # compare failed
        if compare_outputs is not None:
            print("Output file compare failed: here are the details:")
            print_with_indentation("output compare", compare_outputs)
        if compare_errors is not None:
            print("Errors file compare failed: here are the details:")
            print_with_indentation("errors compare", compare_errors)
        return False
    print("passed :)")
    return True


def compare_files(file1, file2):
    """
    compare to files with FC (windows file comparer)
    :param file1:
    :param file2:
    :return: the compaction text if there was errors
    """
    command_to_compare = ['fc', '/W', '/N', '/A', file1, file2]
    code, output, errors = run_with_cmd(command_to_compare)

    if code != 0:  # if code != 0
        print(errors)
        return output
    return None


def run_tests():
    """
    run all the test in the folder test_files\tests with both data folders (simple, complex)
    :return: true iff all passed
    """
    all_passed = True
    tests_invalid_trees = [t for t in listdir(path_to_invalid_trees)]
    tests_no_tree = [t for t in listdir(path_to_no_trees)]

    number_of_tests = len(num_of_parm) + len(invalid) + len(
        tests_invalid_trees) + len(tests_no_tree) +len(valid)

    print("start", number_of_tests, "tests!\nGood luck!\n\n")
    passed_tests = 0

    print(
        "\n********************\nInvalid number of parameters:\n********************\n")
    for name in num_of_parm:  # each test
        if run_one_invalid_test(name, num_of_parm[name], num_of_parm_file):
            passed_tests += 1
        else:
            all_passed = False
        print()

    print(
        "\n********************\nChecking invalid parameters:\n********************\n")
    for name in invalid:  # each test
        if run_one_invalid_test(name, invalid[name], invalid_file):
            passed_tests += 1
        else:
            all_passed = False
        print()

    print(
        "\n********************\nChecking invalid tree files:\n********************\n")
    for name in tests_invalid_trees:  # each test
        if run_one_invalid_test(name, path.join(path_to_invalid_trees,
                                                name) + " 1 2", invalid_file):
            passed_tests += 1
        else:
            all_passed = False
        print()

    print("\n********************\nChecking non-tree graphs:\n********************\n")
    for name in tests_no_tree:  # each test
        if run_one_invalid_test(name,
                                path.join(path_to_no_trees, name) + " 1 2",
                                no_tree_file):
            passed_tests += 1
        else:
            all_passed = False
        print()

    print(
        "\n********************\nChecking valid trees:\n********************\n")
    for name in valid:  # each test
        if run_one_good_test(name, valid[name]):
            passed_tests += 1
        else:
            all_passed = False
        print()


    print("\n********************")

    if all_passed:
        print("All tests passed!!")
        return True
    else:
        print("passes", passed_tests, "out of", number_of_tests, "tests")
        return False


def passed_all():
    print(ASCII_PASSES)
    print("you passed everything!!! \ngo get some sleep")
    # startfile(name_of_p_file)


if __name__ == "__main__":
    while True:
        print("starting tester for ex2 version", tester_ver, '\n')
        compile_file()
        tests_passed = run_tests()

        if tests_passed:
            passed_all()
        else:
            print(ASCII_ERROR)

        input("press enter to restart the tester")
        print('\n\n\nRestarting...')
