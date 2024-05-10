from django.db import models
from django.contrib.auth.models import User

# Create your models here.
class UserInfo(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    photo = models.URLField(max_length=256, blank=True)

    def __str__(self):
        return str(self.user)


class Project(models.Model):
    name = models.CharField(max_length=50)
    owner = models.CharField(max_length=50)
    description = models.CharField(max_length=200)
    stars = models.IntegerField(default=0)
