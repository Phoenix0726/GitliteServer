# Generated by Django 3.2.12 on 2024-05-07 12:33

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('database', '0001_initial'),
    ]

    operations = [
        migrations.RenameModel(
            old_name='User',
            new_name='UserInfo',
        ),
    ]