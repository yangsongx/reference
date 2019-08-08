from django.shortcuts import render
from django.http import HttpRequest, HttpResponse,HttpResponseRedirect,JsonResponse
import rest_framework
from rest_framework.decorators import api_view
from oauth2_provider.decorators import protected_resource
import oauth2_provider
import json
# Create your views here.



# 2019-08-07 Below is showing a legacy view function
# we try embeded it to DRF, with @api_view decorator
@api_view()
def legacy_view_entry(request):
    print("hello")
    data = {
        'code': 0,
        'msg': 'hello world'
    }
    return HttpResponse(json.dumps(data))

# 2019-08-07 Below is showing a legacy view function
# we try embeded it to DRF/Oauth, with  decorator

@protected_resource()
def study_entry(request):
    print("hello from oauth client credential")
    data = {
        'code': 0,
        'msg': 'oauth'
    }
    return HttpResponse('oauth case')

# 2019-08-07 below just use class to demo the Oauth case
#
# class MyFoo(rest_framework.views.APIView):
#     authentication_classes = [oauth2_provider.contrib.rest_framework.OAuth2Authentication]
#     permission_classes = [oauth2_provider.contrib.rest_framework.TokenHasScope]
#     required_scopes = ['read']

#     def get(self, request):
#         print("HaHa")
#         return HttpResponse('class case')


class CityInsightPush(rest_framework.views.APIView):

    def get(self, request):
        print("HaHa")
        return HttpResponse('class case for CI push')

    def post(self, request):
        """
        Just for POST method
        ---
        parameters:
            - name: sn
              type: string
        """
        print("Gaga")
        return HttpResponse('POST class case for CI push')

