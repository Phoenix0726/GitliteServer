import os
from django.http import JsonResponse


def get_filelist(request):
    path = request.GET.get('path')
    
    if not os.path.exists(path):
        return JsonResponse({
            'result': "The file path does not exist",
        })

    filenames = os.listdir(path)
    filelist = {}
    filelist['..'] = 'dir'
    for filename in filenames:
        filepath = os.path.join(path, filename)
        filelist[filename] = 'dir' if os.path.isdir(filepath) else 'file'

    return JsonResponse({
        'result': "success",
        'filelist': filelist,
    })


def get_file_content(request):
    path = request.GET.get('path')

    if not os.path.exists(path):
        return JsonResponse({
            'result': "The file path does not exist",
        })

    if not os.path.isfile(path):
        return JsonResponse({
            'result': "Unable to open the file",
        })

    with open(path, 'r') as file:
        content = file.read()

    return JsonResponse({
        'result': "success",
        'content': content,
    })
