
#include "Core/Engine.h"
#include "Render/Render.h"
#include "Common/Mesh.h"
#include "Common/Image.h"
#include "Common/Gui.h"
#include "Common/Shader.h"
#include "Physic/Physic.h"
#include "Camera.h" 

class CEngineApp : public CEngine
{
public:
	CEngineApp() : CEngine(DIRECT3D9_RENDER)
	{
		start = 0;
	};
	~CEngineApp(){};
	void onKeyPressed(int key);
	void onButtonPressed(int32 key);
	void onButtonUnPressed(int32 key);
	void onKeyUnPressed(int32 key);
	void onMouseMove();
	void Init();
	void DeInit();
	void Render();
private:
	CGuiManager *gui;
	CShader m_pShader;
	CPhysic *physic;
	CCamera cam;

	CRender::FILLMODE fill_type;

	uint32 button,checkbox,radio1,radio2,slider1,label1,label2,label3,edit;
	Bool start,physic_switch;
	uint32 material1;
	uint32 light1,light2,light3,light4;
	uint32 mesh,mesh2,body,test;
	uint32 body_tex,test_tex;
	uint32 jpg_tex,png_tex,tga_tex,dds_tex;
	uint32 default_skin;
	uint32 quad;
	uint32 quad2d,quad2d_2;
	uint32 cursor,cursor1;
	uint32 window,window2;
	uint32 vslider1;
	uint32 scroll_area;
	uint32 text_area;
	uint32 cg_shader,hlsl_shader;
	uint32 model_view_matrix_param,world_matrix,tex0,tex1,light_pos;
	uint32 hlsl_model_view_matrix_param,hlsl_world_matrix;

	uint32 mouse_button;

}engine;

void CEngineApp::DeInit()
{
	SAFE_DELETE(gui);
	SAFE_DELETE(physic);
}

void CEngineApp::Init()
{
	fill_type = CRender::FILLMODE::SOLID;
	SetMouseCursorVisible(0);

	physic_switch = true;

	gui = new CGuiManager();
	physic = new CPhysic();


	default_skin = gui->AddSkin();
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/button.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/button_moved.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/button_pressed.png"));

	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_CHECK_BOX,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/checkbox_unchecked.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_CHECK_BOX_CHECKED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/checkbox_checked.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_RADIO_BUTTON_BOX,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/radiobutton_unchecked.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_RADIO_BUTTON_BOX_CHECKED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/radiobutton_checked.png"));

	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BAR,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/hslider.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/hslider_button.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BAR_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/hslider_pressed.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BUTTON_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/hslider_button_pressed.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BAR_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/hslider_moved.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BUTTON_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/hslider_button_moved.png"));

	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BAR,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/vslider.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/vslider_button.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BAR_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/vslider_pressed.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BUTTON_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/vslider_button_pressed.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BAR_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/vslider_moved.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BUTTON_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/vslider_button_moved.png"));

	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/edit.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/edit.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/edit.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_READ_ONLY,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/edit.png"));

	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_CURSOR,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/pointer.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_CURSOR_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/edit_cursor.png"));

	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/window.png"));

	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,Str("data/window_center.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,Str("data/window_border_t.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,Str("data/window_border_b.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,Str("data/window_border_r.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,Str("data/window_border_l.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,Str("data/window_corner_tr.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,Str("data/window_corner_tl.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,Str("data/window_corner_br.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,Str("data/window_corner_bl.png"));

	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,Str("data/window_center.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,Str("data/window_border_t.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,Str("data/window_border_b.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,Str("data/window_border_r.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,Str("data/window_border_l.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,Str("data/window_corner_tr.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,Str("data/window_corner_tl.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,Str("data/window_corner_br.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,Str("data/window_corner_bl.png"));
	

	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBAR,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_bar_hor.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBAR,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_bar_ver.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_button_hor.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBUTTON,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_button_ver.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_UP,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_up.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_DOWN,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_down.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_LEFT,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_left.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_right.png"));

	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBUTTON_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_button_hor_moved.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBUTTON_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_button_ver_moved.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_UP_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_up_moved.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_DOWN_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_down_moved.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_LEFT_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_left_moved.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT_OVER,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_right_moved.png"));
	
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBUTTON_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_button_hor_pressed.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBUTTON_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_button_ver_pressed.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_UP_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_up_pressed.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_DOWN_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_down_pressed.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_LEFT_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_left_pressed.png"));
	gui->AddTexture(default_skin,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT_PRESSED,CGuiManager::GUISKINPART::GUI_SKIN_PART_SOLID,Str("data/scroll_arow_right_pressed.png"));


	gui->CreateSkin(default_skin,m_pRender);
	gui->CreateFont(m_pRender," `1234567890-=~!@#$%^&*()_+QWERTYUIOPqwertyuiop[]\\{}|ASDFGHJKLasdfghjkl;':\"ZXCVBNMzxcvbnm,./<>?","data/test.ttf",16,2,512,512);

	label1 = gui->CreateLabel(true,CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_NONE,0,2,5,20,m_pRender->GetVendor(),CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	label2 = gui->CreateLabel(false,CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_NONE,0,2,5,200,Str("aaaa"),CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	label3 = gui->CreateLabel(false,CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_NONE,0,2,5,200,Str("aaaas"),CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	
	button = gui->CreateButton(false,100,50,CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	checkbox = gui->CreateCheckBox(20,20,CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	radio1 = gui->CreateRadioButton(20,20,CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	radio2 = gui->CreateRadioButton(20,20,CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	gui->SetGuiElementGroupId(radio1,1);
	gui->SetGuiElementGroupId(radio2,1);
	slider1 = gui->CreateSlider(false,0,500,100,10,30,20,CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	vslider1 = gui->CreateSlider(true,0,500,10,100,20,30,CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	edit = gui->CreateEdit(false,100,20,10,2,Str("edit"),CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	cursor = gui->CreateCursor(CGuiManager::GUISKINTYPE::GUI_SKIN_CURSOR,20,20,CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	window = gui->CreateGuiWindow(true,200,200,0,CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_BORDER,default_skin,m_pRender);
	window2 = gui->CreateGuiWindow(true,150,150,0,CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_BORDER,default_skin,m_pRender);
	scroll_area = gui->CreateScrollArea(200,200,20,20,10,10,10,CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);
	text_area = gui->CreateTextArea(false,false,200,200,0,15,15,500,2,2,Str("edit"),CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID,default_skin,m_pRender);

	gui->SetColorToAllGuiElements(Vec4f(255,255,255,255));
	
	if(start == 0)
	{
		cam.PositionCamera(Vec3f(-10,150,-10),Vec3f(1,-0.5,1),Vec3f(0,1,0));
		start = 1;
	}
	m_pRender->SetProjection(45.0f,800/600,0.5f,100000.0f);
	m_pRender->OnOffDepthTest(1);

	m_pRender->OnOffCulling(1);
	m_pRender->SetCulling(CRender::CULLING::CULL_BACK);

	m_pRender->SetFillMode(fill_type);

	CImage img1;
	img1.LoadImageFromFile("data/texture_types/test.png");
	jpg_tex = m_pRender->CreateTexture(CRender::TEXTURE_TYPE_2D,img1.GetWidth(),img1.GetHeight(),img1.GetChannelCount(),img1.GetFormat(),img1.GetDepth(),0,0);
	m_pRender->FillTexture(jpg_tex,img1.GetData(),0,img1.GetWidth(),img1.GetHeight());

	CImage img2;
	img2.LoadImageFromFile("data/texture_types/test.tga");
	tga_tex = m_pRender->CreateTexture(CRender::TEXTURE_TYPE_2D,img2.GetWidth(),img2.GetHeight(),img2.GetChannelCount(),img2.GetFormat(),img2.GetDepth(),0,0);
	m_pRender->FillTexture(tga_tex,img2.GetData(),0,img2.GetWidth(),img2.GetHeight());

	CImage img3;
	img3.LoadImageFromFile("data/texture_types/test.png");
	png_tex = m_pRender->CreateTexture(CRender::TEXTURE_TYPE_2D,img3.GetWidth(),img3.GetHeight(),img3.GetChannelCount(),img3.GetFormat(),img3.GetDepth(),0,0);
	m_pRender->FillTexture(png_tex,img3.GetData(),0,img3.GetWidth(),img3.GetHeight());

	CImage img4;
	img4.LoadImageFromFile("data/texture_types/test.dds");
	dds_tex = m_pRender->CreateTexture(CRender::TEXTURE_TYPE_2D,img4.GetWidth(),img4.GetHeight(),img4.GetChannelCount(),img4.GetFormat(),img4.GetDepth(),0,0);
	m_pRender->FillTexture(dds_tex,img4.GetData(),0,img4.GetWidth(),img4.GetHeight());


	mesh = m_pRender->CreateGeometry();
	m_pRender->AddVertexDeclaration(mesh,CRender::VERTEXFORMAT::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	m_pRender->AddVertexDeclaration(mesh,CRender::VERTEXFORMAT::VERTEX_FORMAT_NORMAL,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	m_pRender->AddVertexDeclaration(mesh,CRender::VERTEXFORMAT::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	

	
	light1 = m_pRender->CreateLight();
	material1 = m_pRender->CreateMaterial();


	CMesh m2;
	m2.LoadMeshFromFile3DS("data/plane.3ds");
	m2.BuildNormals();

	m_pRender->AddVerexToGeometry(mesh,m2.GetNumVertex(),m2.Vertex);
	m_pRender->AddNormalToGeometry(mesh,m2.GetNumVertex(),m2.Normal);
	m_pRender->AddTexCoordsToGeometry(mesh,m2.GetNumVertex(),m2.TexUV);
	m_pRender->AddIndexToGeometry(mesh,m2.GetNumTriangles()*3,m2.Index);
	m_pRender->SetStartNewGeometry(mesh);

	m_pRender->CreateGeometryVertexBuffer(mesh,CRender::STATIC);
	m_pRender->LoadToVertexBufferGeometry(mesh);
	m_pRender->CreateGeometryIndexBuffer(mesh,CRender::STATIC,CRender::INDEX_FORMAT16);
	m_pRender->LoadToIndexBufferGeometry(mesh);

	m_pRender->SetShadeModel(CRender::SHADEMODEL::SMOOTH);
	//m_pRender->SetFillMode(CRender::FILLMODE::WIREFRAME);
	//shader
	/*
	m_pRender->EnableShadersDebugMode();
	m_pRender->SetShaderLanguage(CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG);
	cg_shader = m_pShader.CreateShaderFromFile(Str("data/shaders/cg.shader"),Str(""),Str(""),m_pRender);
	m_pRender->LoadShader(cg_shader);
	model_view_matrix_param = m_pRender->AddShaderVertexParameter(cg_shader,Str("ModelViewProj"));
	world_matrix = m_pRender->AddShaderVertexParameter(cg_shader,Str("World"));
	tex0 = m_pRender->AddShaderFragmentParameter(cg_shader,Str("tex0"));
	tex1 = m_pRender->AddShaderFragmentParameter(cg_shader,Str("tex1"));
	light_pos = m_pRender->AddShaderFragmentParameter(cg_shader,Str("light_pos"));
	m_pRender->SetShaderParameterTexture(cg_shader,tex0,dds_tex);
	m_pRender->SetShaderParameterTexture(cg_shader,tex1,test_tex);
	*/

	//physic
	/*
	uint32 sp = 0;

	float friction = 0.9f;
	
	physic->AddRigidBody(RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX,Vec3f(5000,5,5000), FLT_MAX);
	physic->GetRigidBody(0)->SetPosition(Vec3f(100, 100, 100));
	physic->GetRigidBody(0)->SetFriction(friction);
	
	
	Float32 masss = 0;
	
	
	for(int i = 1; i<10; i++)
	{
		masss = 1.0f;
		if(i==1)masss = FLT_MAX;
		if(i==9)masss = FLT_MAX;

		sp = physic->AddRigidBody(RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX,Vec3f(10,10,10), masss);
		physic->GetRigidBody(sp)->SetPosition(Vec3f(0, 200,i*15.0f));	
		physic->GetRigidBody(sp)->SetFriction(friction);

		if(i>1)physic->AddJointDistance(physic->GetRigidBody(i),Vec3f(0,0,-5.0),physic->GetRigidBody(i-1),Vec3f(0,0,5.0),20);	
	}

	for(int i = 10; i<19; i++)
	{
		masss = 1.0f;
		if(i==10)masss = FLT_MAX;
		if(i==18)masss = FLT_MAX;

		sp = physic->AddRigidBody(RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX,Vec3f(10,10,10), masss);
		physic->GetRigidBody(sp)->SetPosition(Vec3f(50, 200,(i-9.0f)*15.0f));	
		physic->GetRigidBody(sp)->SetFriction(friction);

		if(i>10)physic->AddJointBall(physic->GetRigidBody(i),Vec3f(0,0,-1.0),physic->GetRigidBody(i-1),Vec3f(0,0,5.0),20);
		
	}

	for(int i = 19; i<29; i++)
	{
		masss = 1.0f;
		if(i==19)masss = FLT_MAX;
		//if(i==28)masss = FLT_MAX;

		sp = physic->AddRigidBody(RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX,Vec3f(10,10,10), masss);
		physic->GetRigidBody(sp)->SetPosition(Vec3f(100.0f, 200.0f,(i-18)*15.0f));	
		physic->GetRigidBody(sp)->SetFriction(friction);

		if(i>19)physic->AddJointHinge(physic->GetRigidBody(i),physic->GetRigidBody(i-1) ,Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f,0.0f,0.0f), 1.0f, 0.1f, 3.0f, 3.0f, 0.1f);
		
	}
	

	for(int j=0;j<1;j++)
	{
		for(int i=0;i<20;i++)
		{
			Float32 s = 20;//300 - i*20;
			sp = physic->AddRigidBody(RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX,Vec3f(s,10,s), 1.0f);
			physic->GetRigidBody(sp)->SetPosition(Vec3f(j*35.0f, 126.0f+i*10,235.0f));	
			physic->GetRigidBody(sp)->SetFriction(friction);
		}
	}
	*/
}

void CEngineApp::Render()
{
	m_pRender->SetFillMode(fill_type);
	cam.CheckForMovement(1.0f);
	if(mouse_button == 2)
	{
		cam.SetViewByMouse(1.0f);
	}
	
	m_pRender->SetLook(cam.Position,Vec3f(0,1,0),cam.View);
	
	/*
	m_pRender->SetMaterialAmbient(material1,Vec4f(1,1,1,255));
	m_pRender->SetMaterialDiffuse(material1,Vec4f(1,1,1,255));
	m_pRender->SetMaterial(material1);
	m_pRender->SetLightType(light1,CRender::LIGHTTYPE::LIGHT_TYPE_POINT);
	m_pRender->SetLightAmbient(light1,Vec4f(1.0f,255.0f,1.0f,255));
	m_pRender->SetLightDiffuse(light1,Vec4f(255,255,255,255));
	m_pRender->SetLightPosition(light1,Vec3f(50.0f,10,50.0f));
	m_pRender->SetLight(light1);
	m_pRender->SetLightEnable(light1);
	*/
	
	

	m_pRender->OnOffLighting(0);

	

	m_pRender->SetDrawable(mesh);
	m_pRender->SetIndexBuffer(mesh,0);

	Float32 tex_shift = 60;

	m_pRender->BindTexture(jpg_tex,0);
	m_pRender->PushMatrix();
	m_pRender->TranslateScale3D(-tex_shift,0,-tex_shift,1.0f,1.0f,1.0f);
	m_pRender->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_LIST,mesh,0);
	m_pRender->PopMatrix();
	m_pRender->UnBindTexture(jpg_tex,0);

	m_pRender->BindTexture(tga_tex,0);
	m_pRender->PushMatrix();
	m_pRender->TranslateScale3D(tex_shift,0,-tex_shift,1.0f,1.0f,1.0f);
	m_pRender->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_LIST,mesh,0);
	m_pRender->PopMatrix();
	m_pRender->UnBindTexture(tga_tex,0);

	m_pRender->BindTexture(png_tex,0);
	m_pRender->PushMatrix();
	m_pRender->TranslateScale3D(-tex_shift,0,tex_shift,1.0f,1.0f,1.0f);
	m_pRender->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_LIST,mesh,0);
	m_pRender->PopMatrix();
	m_pRender->UnBindTexture(png_tex,0);
	
	m_pRender->BindTexture(dds_tex,0);
	m_pRender->PushMatrix();
	m_pRender->TranslateScale3D(tex_shift,0,tex_shift,1.0f,1.0f,1.0f);
	m_pRender->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_LIST,mesh,0);
	m_pRender->PopMatrix();
	m_pRender->UnBindTexture(dds_tex,0);
	
	//shader
	/*
	Mat4f w2;	
	w2.identity();
	//Cg
	m_pRender->SetShaderLanguage(CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG);
	m_pRender->BindShader(cg_shader);
	
	w2.translate(100,0,-30);
	m_pRender->SetShaderMatrixParameterf(cg_shader,world_matrix,w2);
	m_pRender->SetShaderMatrixParameterf(cg_shader,model_view_matrix_param,m_pRender->GetTransformStateMatrix(CRender::TRANSFORMSTATES::PROJECTION) * m_pRender->GetTransformStateMatrix(CRender::TRANSFORMSTATES::VIEW));
	
	m_pRender->SetDrawable(mesh);
	m_pRender->SetIndexBuffer(mesh,0);
	
	m_pRender->BindTexture(dds_tex,0);
	m_pRender->BindTexture(test_tex,1);
	m_pRender->PushMatrix();
	m_pRender->TranslateScale3D(100,0,-30,1.0f,1.0f,1.0f);
	m_pRender->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_LIST,mesh,0);
	m_pRender->PopMatrix();
	m_pRender->UnBindTexture(dds_tex,0);
	m_pRender->UnBindTexture(test_tex,1);
	m_pRender->UnBindShader(cg_shader);
	*/
	
	//physic
	/*
	if(physic_switch)physic->Step(1.0f/60.0f,m_pRender);

	m_pRender->UnBindTexture(dds_tex,0);

	for(uint32 i=0;i<physic->GetRigidBodysCount();i++)
	{
		//physic->GetRigidBody(i)->DrawContacts(m_pRender);
		m_pRender->PushMatrix();
		m_pRender->SetTransformMatrix(physic->GetRigidBody(i)->GetTransform());
		physic->GetRigidBody(i)->GetGeometry()->Draw(m_pRender);	
		m_pRender->PopMatrix();
	}

	for(uint32 i=0;i<physic->GetJointsCount();i++)
	{
		physic->GetJoint(i)->Draw(m_pRender);
	}
	*/
	
	gui->PreDraw();
	Str fps;
	Str fps2;
	//fps.Printf("fps:%f\\nsolver  :%d\\nbroad  :%d\\nnarrow:%d\\ntotal   :%f\\nbroad_pairs: %d\\nnarrow_pairs: %d\\ncontacts: %d\\nbodys num:%d",CalculateFrameRate(),physic->GetSolverTime(),physic->GetBroadPhaseTime(),physic->GetNarrowPhaseTime(),Float32(physic->GetTotalTime()/1000.0f),physic->GetBroadPhasePairs(),physic->GetNarrowPhasePairs(),physic->GetContactsCount(),physic->GetRigidBodysCount());
	fps.Printf("fps:%f",CalculateFrameRate());
	gui->GetGuiElement(label2)->SetLabelText(fps);
	fps2.Printf("sss");
	gui->GetGuiElement(label3)->SetLabelText(fps2);

	
	gui->SetDrawPosition(label2,10,0);
	gui->SetDrawPosition(label1,10,20);

	gui->SetDrawPosition(button,200,400);
	gui->SetDrawPosition(text_area,200,400);
	gui->SetDrawPosition(vslider1,650,450);
	gui->SetDrawPosition(slider1,690,450);
	gui->SetDrawPosition(scroll_area,400,400);
	gui->SetDrawPosition(edit,50+gui->GetGuiElement(slider1)->GetSliderPosition(),50+gui->GetGuiElement(vslider1)->GetSliderPosition(),scroll_area);
	gui->SetDrawPosition(button,50+gui->GetGuiElement(slider1)->GetSliderPosition(),0+gui->GetGuiElement(vslider1)->GetSliderPosition(),scroll_area);
	gui->SetDrawPosition(window,0,400);
	gui->SetDrawPosition(checkbox,0,0,window);
	gui->SetDrawPosition(radio1,20,0,window);
	gui->SetDrawPosition(radio2,40,0,window);
	gui->SetDrawPosition(window2,50,50,window);
	gui->SetDrawPosition(label3,0,0,window2);

	gui->DrawAll(default_skin,GetMouseX(),GetMouseY(),mouse_button,m_pRender);	

}

void CEngineApp::onKeyUnPressed(int32 key)
{
}

void CEngineApp::onKeyPressed(int key)
{
	if(key == CInput::KEY_ESCAPE)
	{
		PostQuitMessage(0);
	}

	if(key == CInput::KEY_DELETE){gui->DeleteCharForAll();}
	if(key == CInput::KEY_RETURN){gui->EnterCharForAll();}
	if(key == CInput::KEY_BACK){gui->BackspaceCharForAll();}
	if(key != CInput::KEY_BACK && key != CInput::KEY_DELETE){gui->AddCharForAll(gui->GetCharByIndex(key));}
	if(key == CInput::KEY_SPACE){gui->SpaceCharForAll();}
	if(key == CInput::KEY_LEFT){gui->CursorLeftForAll();}
	if(key == CInput::KEY_RIGHT){gui->CursorRightForAll();}
	if(key == CInput::KEY_UP){gui->CursorUpForAll();}
	if(key == CInput::KEY_DOWN){gui->CursorDownForAll();}
	if(key == CInput::KEY_HOME){gui->CursorHomeForAll();}
	if(key == CInput::KEY_END){gui->CursorEndForAll();}

	if(key == CInput::KEY_F9)
	{
		DeInit();
		if(GetRenderMode() == CEngine::OPENGL_RENDER)
		{
			SetRenderMode(CEngine::DIRECT3D9_RENDER);
		}
		else if(GetRenderMode() == CEngine::DIRECT3D9_RENDER)
		{
			SetRenderMode(CEngine::OPENGL_RENDER);
		}
		Init();
	}

	if(key == CInput::KEY_F8)
	{
		fill_type = (fill_type == CRender::FILLMODE::SOLID) ? CRender::FILLMODE::WIREFRAME : CRender::FILLMODE::SOLID;
	}

	if(key == CInput::KEY_SPACE)
	{
		physic_switch = (physic_switch == true) ? false : true;
	}

	if(key == CInput::KEY_INSERT)
	{
		uint32 sp = 0;
		sp = physic->AddRigidBody(RIGID_BODY_TYPE::RIGID_BODY_TYPE_CYLINDER,Vec3f(5,5,5), 1.0f);
		physic->GetRigidBody(sp)->SetPosition(cam.Position);	
		physic->GetRigidBody(sp)->SetFriction(0.9f);
		physic->GetRigidBody(sp)->SetVelocity(-(cam.Position - cam.View)*100);
	}
}

void CEngineApp::onButtonPressed(int32 key)
{
	
	if(key == CInput::KEY_LMOUSEBUTTON)
	{
		mouse_button = 1;
		gui->CalculateStateForAll(edit,GetMouseX(),GetMouseY(),mouse_button);
	}

	if(key == CInput::KEY_RMOUSEBUTTON)
	{
		mouse_button = 2;
	}
	
}

void CEngineApp::onButtonUnPressed(int32 key)
{
	mouse_button = 0;
	gui->CalculateStateForAll(edit,GetMouseX(),GetMouseY(),mouse_button);
}

void CEngineApp::onMouseMove()
{
	gui->CalculateStateForAll(edit,GetMouseX(),GetMouseY(),mouse_button);
}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return engine.EngineWinProc(hWnd,msg,wParam,lParam);
};

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmdLine, INT cmdShow )
{
	engine.SetMsgProc(MsgProc);
    return engine.EngineRun(hInst,hPrev,cmdLine,cmdShow);
};
