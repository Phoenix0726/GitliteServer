import os
from django.http import JsonResponse

from database.models import UserInfo, Project
from .setttings import base_dir


def get_projects(request, username=None, project=None):
    data = request.GET
    if username is None:
        username = data.get('username', "").strip()
    if not username:
        return JsonResponse({
            'result': "The user doesn't exist",
        })
    
    projects = []
    for p in Project.objects.filter(owner=username):
        project = {
            'name': p.name,
            'owner': p.owner,
            'description': p.description,
            'stars': p.stars,
        }
        projects.append(project)
    
    return JsonResponse({
        'result': "success",
        'projects': projects,
    })


def create_project(request, username=None, project=None):
    user = request.user
    if not user.is_authenticated:
        return JsonResponse({
            'result': "not login",
        })
    
    data = request.GET
    name = data.get('name', "").strip()
    owner = data.get('owner', "").strip()
    description = data.get('description', "").strip()

    if not name or not owner:
        return JsonResponse({
            'result': "项目名和作者不能为空",
        })
    
    if not UserInfo.objects.filter(user__username=owner).exists():
        return JsonResponse({
            'result': "用户不存在",
        })
    
    if owner != user.username:
        return JsonResponse({
            'result': "not login",
        })
    
    if Project.objects.filter(name=name, owner=owner).exists():
        return JsonResponse({
            'result': "项目名已存在",
        })
    
    Project.objects.create(name=name, owner=owner, description=description)

    return JsonResponse({
        'result': "success",
    })


def delete_project(request, username=None, project=None):
    user = request.user
    if not user.is_authenticated:
        return JsonResponse({
            'result': "not login",
        })

    data = request.GET
    name = data.get('name', "").strip()
    owner = data.get('owner', "").strip()

    if owner != user.username:
        return JsonResponse({
            'result': "不能删除别人的项目!",
        })
    
    Project.objects.filter(name=name, owner=owner).delete()

    return JsonResponse({
        'result': "success",
    })
