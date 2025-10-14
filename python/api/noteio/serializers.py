from django.contrib.auth.models import Group, User

from rest_framework import serializers

from .models import Todo
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
    user = serializers.HyperlinkedRelatedField(view_name='user-detail', read_only=True)
    user_id = serializers.IntegerField(source='user.id', read_only=True)

    class Meta:
        model = Todo
        fields = ['id', 'url', 'user', 'user_id', 'title', 'text', 'completed', 'created_on', 'updated_on', 'due']
        read_only_fields = ['created_on', 'updated_on']