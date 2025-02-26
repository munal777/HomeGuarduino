from django.urls import path
from .views import SensorDataView, notificationView

urlpatterns = [
    path('alert/', SensorDataView.as_view(), name='alert'),
    path('index/', notificationView, name='notificationdata')
]