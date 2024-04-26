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

from gitlitehub.views.index import index
from gitlitehub.views.files_info import get_filelist
from gitlitehub.views.files_info import get_file_content
from gitlitehub.views.files_info import get_branches
from gitlitehub.views.files_info import get_commits
from gitlitehub.views.files_info import checkout_branch
from gitlitehub.views.files_info import checkout_commit


urlpatterns = [
    path('admin/', admin.site.urls),
    path("", index, name="index"),
    path("get_filelist/", get_filelist, name="get_filelist"),
    path("get_file_content/", get_file_content, name="get_file_content"),
    path("get_branches/", get_branches, name="get_branches"),
    path("get_commits/", get_commits, name="get_commit"),
    path("checkout_branch/", checkout_branch, name="checkout_branch"),
    path("checkout_commit/", checkout_commit, name="checkout_commit"),
]
