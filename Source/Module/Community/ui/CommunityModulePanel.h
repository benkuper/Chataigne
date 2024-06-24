/*
  ==============================================================================

	CommunityModulePanel.h
	Created: 22 Jun 2024 5:29:38pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class CommunityModulePanel :
	public ShapeShifterContentComponent
{
public:
	CommunityModulePanel(const String& name);
	~CommunityModulePanel();

	void resized() override;
	void mouseDown(const MouseEvent& e) override;

	class WebComp :
		public WebBrowserComponent
	{
	public:
		WebComp(var initData);
		~WebComp() {}
		String modulesURL;

		bool pageAboutToLoad(const String& url) override;
		void pageFinishedLoading(const String& url) override;
		bool pageLoadHadNetworkError(const String& error) override;
	};

	std::unique_ptr<WebComp> webComp;

	static CommunityModulePanel* create(const String& name) { return new CommunityModulePanel(name); }
	static const String getPanelName() { return "Community Modules"; }

};
