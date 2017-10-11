const dir = currentDir() + '/somefiles';
createDir(dir);

files = 10;
for (var i = 0; i < files; i++) {
	text_string = 'Строка номер ' + (files - i - 1) + '\nЗаписана в файл номер ' + (i) + '\n';
	filename = '/file_' + i;

	writeFile(dir + filename, text_string);
}
