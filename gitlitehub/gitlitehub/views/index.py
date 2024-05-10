from django.shortcuts import render


def index(request):
    return render(request, 'index.html')


def user(request, username):
    context = {
        'username': username,
    }
    return render(request, 'index.html', context)


def project(request, username, project):
    context = {
        'username': username,
        'project': project,
    }
    return render(request, 'index.html', context)
