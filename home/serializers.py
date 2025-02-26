from rest_framework import serializers
from django.utils import timezone
from .models import SensorData

class SensorDataSerializer(serializers.Serializer):
    gas_detected = serializers.IntegerField(required=False)
    motion_detected = serializers.BooleanField(required=False)
    door_open = serializers.BooleanField(required=False)
    timestamp = serializers.DateTimeField(default=timezone.now)

class SensorDataModelSerializer(serializers.ModelSerializer):

    class Meta:
        model= SensorData
        fields= '__all__'
        read_only_fields = ['timestamp'] 