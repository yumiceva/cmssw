__author__="Aurelija"
__date__ ="$2010-07-15 12.27.32$"

import re
from os.path import join
from Utilities.ReleaseScripts.cmsCodeRules.pathToRegEx import pathesToRegEx, pathToRegEx

def getFilePathesFromWalk(osWalkResult, file, exceptPathes = []):

    listOfFiles = []

    file = pathToRegEx(file)

    for root, dirs, files in osWalkResult:
        for name in files:
            excepted = False
            fullPath = join(root,name)
            for path in pathesToRegEx(exceptPathes):
                if re.match(path, fullPath):
                    excepted = True
                    break
            if not excepted and re.match(file, name):
                listOfFiles.append(fullPath)
    return listOfFiles
