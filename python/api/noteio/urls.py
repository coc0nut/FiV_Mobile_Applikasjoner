from django.urls import path, include
from rest_framework import routers

from .views import UserViewSet, GroupViewSet, TodoViewSet, CurrentUserView
from rest_framework_simplejwt.views import (
    TokenObtainPairView,
    TokenRefreshView,
)

router = routers.DefaultRouter()
router.register(r'users', UserViewSet, basename='user')
router.register(r'groups', GroupViewSet, basename='group')
router.register(r'todos', TodoViewSet, basename='todo')

urlpatterns = [
    path('', include(router.urls)),
    path(
        'me/', 
        CurrentUserView.as_view({'get': 'list', 'put': 'update', 'patch': 'partial_update'}), 
        name='current-user'
    ),
    path('api/token/', TokenObtainPairView.as_view(), name='token_obtain_pair'),
    path('api/token/refresh/', TokenRefreshView.as_view(), name='token_refresh'),
    path('api-auth/', include('rest_framework.urls', namespace='rest_framework')),
]