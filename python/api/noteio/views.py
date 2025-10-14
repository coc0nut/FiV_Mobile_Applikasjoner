from rest_framework import permissions, viewsets, status
from rest_framework.exceptions import PermissionDenied
from rest_framework.views import APIView
from rest_framework.response import Response

from django.contrib.auth.models import Group, User
from .serializers import GroupSerializer, UserSerializer, TodoSerializer
from .models import Todo

class CurrentUserView(viewsets.ViewSet):
     permission_classes = [permissions.IsAuthenticated]
     
     def list(self, request):
        serializer = UserSerializer(request.user, context={'request': request})
        return Response(serializer.data)
     
     def update(self, request, pk=None):
        """ Allow updating only email, first_name, last_name for current user"""
        user = request.user
        
        allowed_fields = {'email', 'first_name', 'last_name'}
        update_data = {k: v for k, v in request.data.items() if k in allowed_fields}

        if not update_data:
            return Response(
                {'error', 'No valid fields to update. Allowed: email, first_name, last_name'},
                status=status.HTTP_400_BAD_REQUEST
            )
        
        serializer = UserSerializer(user, data = update_data, partial=True, context={'request': request})
        if serializer.is_valid():
            serializer.save()
            return Response(serializer.data)
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
     
     def partial_update(self, request, pk=None):
         return self.update(request, pk)

class UserViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows users to be viewed
    """

    queryset = User.objects.all().order_by('-date_joined')
    serializer_class = UserSerializer
    permission_classes = [permissions.IsAuthenticated]

class GroupViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows groups to be viewed or edited.
    """

    queryset = Group.objects.all().order_by('name')
    serializer_class = GroupSerializer
    permission_classes = [permissions.IsAuthenticated]


class TodoViewSet(viewsets.ModelViewSet):
    """
    API endpoint for todos created by users.
    """
    queryset = Todo.objects.all()
    serializer_class = TodoSerializer
    permission_classes = [permissions.IsAuthenticated]

    def get_queryset(self):
        """Return todos for the authenticated user (staff can see all)"""
        user = self.request.user
        if user.is_staff:
            return Todo.objects.all()
        return Todo.objects.filter(user=user)
    
    def perform_create(self, serializer):
        """Automatically assign the authenticated user when creating"""
        serializer.save(user=self.request.user)

    def perform_update(self, serializer):
        """Ensure users can only update their own todos"""
        instance = self.get_object()
        if not (self.request.user.is_staff or instance.user == self.request.user):
                raise PermissionDenied("Cannot modify others todos")
        serializer.save()

    def perform_destroy(self, instance):
        """Ensure users can only delete their own todos"""
        if not (self.request.user.is_staff or instance.user == self.request.user):
            raise PermissionDenied("Cannot delete other users todos")
        instance.delete()