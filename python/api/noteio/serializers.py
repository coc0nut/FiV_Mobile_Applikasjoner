from django.contrib.auth.models import Group, User

from rest_framework import serializers

class UserSerializer(serializers.HyperlinkedModelSerializer):
    id = serializers.IntegerField(read_only=True)
    class Meta:
        model = User
        fields = ['id', 'url', 'username', 'email', 'first_name', 'last_name', 'last_login', 'is_superuser', "is_staff", "is_active", "date_joined", 'groups']

class GroupSerializer(serializers.HyperlinkedModelSerializer):
    id = serializers.IntegerField(read_only=True)
    class Meta:
        model = Group
        fields = ['id', 'url', 'name']

class AuthTokenSerializer(serializers.Serializer):
    username = serializers.CharField()
    password = serializers.CharField(write_only=True)

from .models import Todo

class TodoSerializer(serializers.HyperlinkedModelSerializer):
    id = serializers.IntegerField(read_only=True)
    url = serializers.HyperlinkedIdentityField(view_name='todo-detail', read_only=True)
    user_id = serializers.IntegerField(read_only=True)

    class Meta:
        model = Todo
        fields = ['id', 'user_id', 'title', 'text', 'completed', 'created_on', 'updated_on', 'due', 'url']