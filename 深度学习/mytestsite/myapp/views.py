# from django.shortcuts import render
from django.shortcuts import render
from django.http import HttpResponse

def myapp_view(request):
    return render(request, "app/ceshi_template.html")

# Create your views here.
