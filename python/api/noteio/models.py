from django.db import models
from django.contrib.auth import get_user_model
# Create your models here.

User = get_user_model()

class Todo(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='todos')

    title = models.CharField(max_length=255)
    text = models.TextField(blank=True)

    completed = models.BooleanField(default=False)

    created_on = models.DateTimeField(auto_now_add=True)
    updated_on = models.DateTimeField(auto_now=True)

    due = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return f"{self.title} ({self.pk})"

    class Meta:
        ordering = ['-created_on']
