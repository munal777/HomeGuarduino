from django.core.mail import send_mail
from django.conf import settings

def alert_mail(subject, message, recipient):
    from_email = settings.DEFAULT_FROM_EMAIL
    recipient_list = [recipient]

    send_mail(subject, message, from_email, recipient_list)