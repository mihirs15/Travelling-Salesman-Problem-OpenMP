import sys, subprocess

start = int(sys.argv[1])
end = int(sys.argv[2])
p = int(sys.argv[3])
r = int(sys.argv[4])

for problem_size in range(start, end + 1):
	for proc in range(p + 1):
		for runs in range(r):
			if proc == 0:
				subprocess.call('./s1 ' + str(problem_size) + ' 0 ' + ' < ../../TC' + str(problem_size) + '.txt', shell=True)
			else:
				subprocess.call('./p1 ' + str(problem_size) + ' ' + str(proc) + ' < ../../TC' + str(problem_size) + '.txt', shell=True)

