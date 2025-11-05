from django.urls import path, include
from rest_framework import routers

from .views import UserViewSet, GroupViewSet, TodoViewSet, CurrentUserView


router = routers.DefaultRouter()
router.register(r'users', UserViewSet, basename='user')
router.register(r'groups', GroupViewSet, basename='group')
router.register(r'todos', TodoViewSet, basename='todo')

urlpatterns = [
    path('', include(router.urls)),
    path('me/', CurrentUserView.as_view({
        'get': 'list',
        'put': 'update',
        'patch': 'partial_update'
    }), name='current-user'),

]