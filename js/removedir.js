const dir = currentDir() + '/somefiles';

files_arr = readDir(dir);
for (var i = 0; i < files_arr.length; i++) {
	rmFile(dir + '/' + files_arr[i]);
}
rmDir(dir);