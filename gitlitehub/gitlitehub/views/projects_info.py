import os
from django.http import JsonResponse

from .setttings import base_dir


def get_projects(request, username=None, project=None):
    user_dir = os.path.join(base_dir, username)

    if not os.path.exists(user_dir):
        return JsonResponse({
            'result': "The user doesn't exist",
        })
    
    projects = os.listdir(user_dir)

    return JsonResponse({
        'result': "success",
        'projects': projects,
    })