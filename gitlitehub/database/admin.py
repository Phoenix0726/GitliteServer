from django.contrib import admin
from .models import UserInfo, Project

# Register your models here.
class UserInfoAdmin(admin.ModelAdmin):
    list_display = ('user',)


class ProjectAdmin(admin.ModelAdmin):
    list_display = ('name', 'owner', 'description', 'stars')


admin.site.register(UserInfo, UserInfoAdmin)
admin.site.register(Project, ProjectAdmin)

