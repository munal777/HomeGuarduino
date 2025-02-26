from django.db import models

# Create your models here.
class SensorData(models.Model):
    gas_detected = models.IntegerField(null=True, blank=True)  # Optional field
    motion_detected = models.BooleanField(null=True, blank=True)  # Optional field
    door_open = models.BooleanField(null=True, blank=True)  # Optional field
    timestamp = models.DateTimeField(auto_now_add=True)  # Automatically set the timestamp when the object is created

    def __str__(self):
        return f"SensorData(id={self.id}, gas_detected={self.gas_detected}, motion_detected={self.motion_detected}, door_open={self.door_open})"