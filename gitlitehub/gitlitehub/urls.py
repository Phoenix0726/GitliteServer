"""gitlitehub URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path

from gitlitehub.views.index import index, user, project

from gitlitehub.views.files_info import get_filelist, get_file_content, get_branches, get_commits, checkout_branch, checkout_commit

from gitlitehub.views.users_info import register, login, get_info

from gitlitehub.views.projects_info import get_projects, create_project, delete_project


urlpatterns = [
    path('admin/', admin.site.urls),
    path("", index, name="index"),
    
    path("<str:username>/<str:project>/get_filelist/", get_filelist, name="get_filelist"),
    path("<str:username>/<str:project>/get_file_content/", get_file_content, name="get_file_content"),
    path("<str:username>/<str:project>/get_branches/", get_branches, name="get_branches"),
    path("<str:username>/<str:project>/get_commits/", get_commits, name="get_commit"),
    path("<str:username>/<str:project>/checkout_branch/", checkout_branch, name="checkout_branch"),
    path("<str:username>/<str:project>/checkout_commit/", checkout_commit, name="checkout_commit"),

    path("register/", register, name="register"),
    path("login/", login, name="login"),
    path("<str:username>/get_info/", get_info, name="get_info"),
    path("<str:username>/<str:project>/get_info/", get_info, name="get_info"),
    path("<str:username>/get_projects/", get_projects, name="get_projects"),
    path("<str:username>/<str:project>/get_projects/", get_projects, name="get_projects"),
    path("<str:username>/create_project/", create_project, name="create_project"),
    path("<str:username>/<str:project>/create_project/", create_project, name="create_project"),
    path("<str:username>/delete_project/", delete_project, name="delete_project"),
    path("<str:username>/<str:project>/delete_project/", delete_project, name="delete_project"),

    path("<str:username>/", user, name="user"),
    path("<str:username>/<str:project>/", project, name="project"),
]
