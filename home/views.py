from rest_framework.response import Response
from rest_framework.views import APIView
from .utils import alert_mail
from rest_framework import status
from .serializers import SensorDataSerializer, SensorDataModelSerializer
from .models import SensorData
from django.shortcuts import render



def notificationView(request):
    query = SensorData.objects.all().order_by('-timestamp')[:10]

    return render(request, 'index.html', {'query': query})


class SensorDataView(APIView):

    def post(self, request):
        serializers = SensorDataModelSerializer(data = request.data, partial=True)

        if serializers.is_valid():
            gas_level = serializers.validated_data.get('gas_detected')  # None if not provided
            motion_state = serializers.validated_data.get('motion_detected')  # None if not pro
            reed_switch_triggered = serializers.validated_data.get('door_open')
            timestamp = serializers.validated_data.get('timestamp')
            sensor_data = serializers.save() 
            timestamp = sensor_data.timestamp
            print(f"Timestamp: {timestamp}")

            if gas_level is not None:
                subject = 'Gas Leakage Detected at Your Home'
                message = f'Alert: A gas leakage has been detected in your home by the Home Guarduino system. The current gas level is {gas_level}.'
                recipient = 'homeguarduino@gmail.com'

                alert_mail(subject, message, recipient)
            
            if motion_state is not None:

                subject = 'Unwanted Motion Detected in living Room'
                message = f'Motion has been detected by the Home Guarduino system at {timestamp}. Please check the area for any unusual activity.'
                recipient = 'homeguarduino@gmail.com'

                alert_mail(subject, message, recipient)

            if reed_switch_triggered is not None:
                print(f"Door or window state detected: {reed_switch_triggered}")
                subject = 'Door or window is Opened'
                message = f'The door has been opened at {timestamp} detected by the Home Guarduino system. Please verify if this is expected.'
                recipient = 'homeguarduino@gmail.com'

                alert_mail(subject, message, recipient)

            return Response({'state': "success"}, status=status.HTTP_200_OK)
        return Response(serializers.errors, status=status.HTTP_400_BAD_REQUEST)