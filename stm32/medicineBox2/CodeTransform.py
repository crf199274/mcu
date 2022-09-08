import chardet
import codecs
import os
import sys

encode_out = 'utf-8'
 
def code2code(filename):
    fileencoding = chardet.detect(open(filename, "rb").read())
    #print(fileencoding)
    fileencode = fileencoding['encoding']
    if fileencode == encode_out:
        return
    with codecs.open(filename, mode='r', encoding=fileencode) as fi:
        data = fi.read()
    with codecs.open(filename, mode='w', encoding=encode_out) as fo:
        fo.write(data)
    
def main():
    for root, dirs, files in os.walk(sys.path[0]):
        for file in files:
            if os.path.splitext(file)[1] == '.c' or os.path.splitext(file)[1] == '.h':
                code2code(os.path.join(root,file))
                print(os.path.join(root,file))
    print('Finish 转换完毕')
    
if __name__ == '__main__':
    main()