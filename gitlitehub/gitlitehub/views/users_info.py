from django.http import JsonResponse
from django.contrib.auth import authenticate
from django.contrib import auth
from django.contrib.auth.models import User

from database.models import UserInfo


def register(request):
    data = request.GET
    username = data.get('username', "").strip()
    password = data.get('password', "").strip()
    password_confirm = data.get('password_confirm', "").strip()

    if not username or not password:
        return JsonResponse({
            'result': "用户名和密码不能为空",
        })

    if password != password_confirm:
        return JsonResponse({
            'result': "两次输入密码不一致",
        })

    if User.objects.filter(username=username).exists():
        return JsonResponse({
            'result': "用户名已存在",
        })
    
    user = User(username=username)
    user.set_password(password)
    user.save()
    UserInfo.objects.create(user=user, photo="https://logos-download.com/wp-content/uploads/2016/09/GitHub_logo.png")

    auth.login(request, user)

    return JsonResponse({
        'result': "success",
    })


def login(request):
    data = request.GET
    username = data.get('username')
    password = data.get('password')
    user = authenticate(username=username, password=password)

    if not user:
        return JsonResponse({
            'result': "用户名或密码错误",
        })
    
    auth.login(request, user)

    return JsonResponse({
        'result': "success",
    })


def get_info(request):
    user = request.user
    if not user.is_authenticated:
        return JsonResponse({
            'result': "not login",
        })
    else:
        user_info = UserInfo.objects.get(user=user)
        return JsonResponse({
            'result': "success",
            'username': user_info.user.username,
            'photo': user_info.photo,
        })
