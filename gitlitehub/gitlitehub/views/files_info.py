import os
import shutil
import subprocess
from django.http import JsonResponse

from .setttings import base_dir
from .setttings import gitlite


def get_branches(request):
    project_dir = request.GET.get('project')
    project_dir = os.path.join(base_dir, project_dir)

    if not os.path.exists(project_dir):
        return JsonResponse({
            'result': "The project doesn't exist",
        })
    
    branches_dir = os.path.join(project_dir, '.gitlite/refs/heads')
    branches = os.listdir(branches_dir)
    return JsonResponse({
        'result': "success",
        'branches': branches,
    })


def get_commit(request):
    project_dir = request.GET.get('project')
    project_dir = os.path.join(base_dir, project_dir)
    if not os.path.exists(project_dir):
        return JsonResponse({
            'result': "The project doesn't exist",
        })
    
    args = ['log']
    output = subprocess.run([gitlite] + args, cwd=project_dir,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if output.stderr:
        print(output.stderr)

    commit_list = []
    for str in output.stdout.split('\n'):
        if str.startswith("commit"):
            commit_list.append(str[7:])

    return JsonResponse({
        'result': "success",
        'commit_list': commit_list,
    })


def delete_path(path):
    try:
        if os.path.isfile(path):
            os.remove(path)
        elif os.path.isdir(path):
            shutil.rmtree(path)
        else:
            print(f"The path {path} does not exist.")
    except OSError as e:
        print(f"Error: {e.strerror}")


def clear_project(project_dir):
    filenames = os.listdir(project_dir)
    for filename in filenames:
        if filename != ".gitlite":
            filepath = os.path.join(project_dir, filename)
            delete_path(filepath)


def checkout_commit(request):
    project_dir = request.GET.get('project')
    project_dir = os.path.join(base_dir, project_dir)
    if not os.path.exists(project_dir):
        return JsonResponse({
            'result': "The project doesn't exist",
        })
    
    # 先把原本的文件清空
    clear_project(project_dir)
    
    commit = request.GET.get('commit')
    if len(commit) != 40:
        return JsonResponse({
            'result': "This is not a commit id",
        })
    
    args = ['checkout', commit]
    output = subprocess.run([gitlite] + args, cwd=project_dir,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if output.stderr:
        print(output.stderr)

    return JsonResponse({
        'result': "success",
    })


def get_filelist(request):
    path = request.GET.get('path')
    path = os.path.join(base_dir, path)
    
    if not os.path.exists(path):
        return JsonResponse({
            'result': "The file path does not exist",
        })

    filenames = os.listdir(path)
    filelist = {}
    filelist['..'] = 'dir'
    for filename in filenames:
        if filename == '.gitlite':
            continue
        filepath = os.path.join(path, filename)
        filelist[filename] = 'dir' if os.path.isdir(filepath) else 'file'

    return JsonResponse({
        'result': "success",
        'filelist': filelist,
    })


def get_file_content(request):
    path = request.GET.get('path')
    path = os.path.join(base_dir, path)

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
