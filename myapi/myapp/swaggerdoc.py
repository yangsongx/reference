import rest_framework
from rest_framework.schemas import SchemaGenerator

from rest_framework_swagger.renderers import (
        SwaggerUIRenderer,
        OpenAPIRenderer
)

class SwaggerSchemaView(rest_framework.views.APIView):
  permission_classes = [rest_framework.permissions.IsAuthenticated,]
  renderer_classes = [OpenAPIRenderer, SwaggerUIRenderer]

  def get(self, request):
      generator = SchemaGenerator(title = "Title of Your Cool API")
      schema = generator.get_schema(request=request)
      return rest_framework.response.Response(schema)