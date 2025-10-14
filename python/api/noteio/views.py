from rest_framework import permissions, viewsets, status
from rest_framework.exceptions import PermissionDenied
from rest_framework.response import Response
from rest_framework.decorators import action

from django.contrib.auth import authenticate
from django.contrib.auth.models import Group, User
from .serializers import GroupSerializer, UserSerializer, TodoSerializer
from .models import Todo

class CurrentUserView(viewsets.ViewSet):
     """
     Endpoint for current user /me/
     """
     permission_classes = [permissions.IsAuthenticated]
     
     def list(self, request):
        """
        GET user details
        """
        serializer = UserSerializer(request.user, context={'request': request})
        return Response(serializer.data)
     
     def update(self, request, pk=None):
        """
        Allow updating only email, first_name, last_name for current user
        """
        user = request.user
        
        allowed_fields = {'email', 'first_name', 'last_name'}
        update_data = {k: v for k, v in request.data.items() if k in allowed_fields}

        if not update_data:
            return Response(
                {'error': 'No valid fields to update. Allowed: email, first_name, last_name'},
                status=status.HTTP_400_BAD_REQUEST
            )
        
        serializer = UserSerializer(user, data = update_data, partial=True, context={'request': request})
        if serializer.is_valid():
            serializer.save()
            return Response(serializer.data)
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
     
     def partial_update(self, request, pk=None):
         """
         Allow put request
         """
         return self.update(request, pk)
     
     @action(detail=False, methods=['post'], url_path='change-password')
     def change_password(self, request):
         """
        Change password
         """
         user = request.user

         old_password = request.data.get('old_password')
         new_password = request.data.get('new_password')
         confirm_password = request.data.get('confirm_password')

         if not old_password or not new_password or not confirm_password:
             return Response({
                 'error': 'old_password, new_password, and confirm_password are required'
             }, status=status.HTTP_400_BAD_REQUEST)
         
         if not authenticate(username=user.username, password=old_password):
             return Response({
                 'error': 'Authentication failed, wrong old_password'
             }, status=status.HTTP_400_BAD_REQUEST)
         
         if new_password != confirm_password:
             return Response({
                 'error': 'New passwords doesnt match'
             }, status=status.HTTP_400_BAD_REQUEST)
         
         if len(new_password) < 8:
             return Response({
                 'error': 'New password must be at least 8 characters long'
             }, status=status.HTTP_400_BAD_REQUEST)
         
         user.set_password(new_password)
         user.save()

         return Response({
             'message': 'Password changed successfully'
         }, status=status.HTTP_200_OK)

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