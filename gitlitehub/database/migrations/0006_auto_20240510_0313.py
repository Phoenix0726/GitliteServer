# Generated by Django 3.2.12 on 2024-05-10 03:13

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('database', '0005_project'),
    ]

    operations = [
        migrations.RenameField(
            model_name='project',
            old_name='author',
            new_name='owner',
        ),
        migrations.AddField(
            model_name='project',
            name='description',
            field=models.CharField(default='', max_length=200),
        ),
    ]
