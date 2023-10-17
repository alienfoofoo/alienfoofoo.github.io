from django.urls import path
from . import views

urlpatterns = [
    path('', views.myapp_view, name='myapp_view'),
]
