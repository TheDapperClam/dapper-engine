#pragma once

#include "Component.h"
#include "glad/glad.h"
#include <map>
#include <string>
#include <vector>

namespace dap {
	class Shader;
	struct Vertex;

	class RenderObject : public dap::Component {
	protected:
		static std::map<int, std::vector<dap::RenderObject*>> AllRenderers;

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;

	public:
		virtual void ImGuiWidgetsBegin () override;

	public:
		const static std::map<int, std::vector<dap::RenderObject*>>* GetAllRenderers ();

	protected:
		GLuint glDrawMode = GL_TRIANGLES;
		GLuint elementBufferObject = 0;
		GLuint vertexArrayObject = 0;
		GLuint vertexBufferObject = 0;
		std::vector<unsigned int> elements = {};
		std::vector<dap::Vertex> vertices = {};
		dap::Shader* shader = NULL;

	private:
		int layer = 0;

	public:
		RenderObject ( bool processGameFunctions = true );
		virtual ~RenderObject () override;

	public:
		GLuint GetDrawMode ();
		unsigned int GetElementsSize ();
		int GetLayer ();
		dap::Shader* const GetShader ();
		virtual void RefreshTexCoords ();
		void SetLayer ( int newLayer );
		void SetShader ( std::string key );
		virtual bool Use () const;

	protected:
		virtual void BuildElements () = 0;
		virtual void BuildVertices () = 0;
		void BuildGLObject ();
	};
}
