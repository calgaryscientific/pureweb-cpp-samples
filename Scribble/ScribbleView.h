
// ScribbleView.h : interface of the CScribbleView class
//

#pragma once

#include "pureweb.h" // must be first

class CScribbleView : public CView, public CSI::PureWeb::Server::IRenderedView
{
protected: // create from serialization only
    CScribbleView();
    DECLARE_DYNCREATE(CScribbleView)

    // Attributes
public:
    CScribbleDoc* GetDocument() const;

private:
    COLORREF m_cColor;
    HDC m_hdcOffscreen;
    CDC m_dcOffscreen;
    HBITMAP m_hBitmap;
    HBITMAP m_hbitMapOld;    
    CPoint m_startpt;
    CPoint m_endpt;
    int m_Width;
    int m_Height;
    void* m_pPixelBits;

    // Operations
public:
    // PureWeb::IRenderedView
    void SetClientSize(CSI::PureWeb::Size clientSize);
    CSI::PureWeb::Size GetActualSize();
    void RenderView(CSI::PureWeb::Server::RenderTarget target);
    void PostKeyEvent(const CSI::PureWeb::Ui::PureWebKeyboardEventArgs& keyEvent);
    void PostMouseEvent(const CSI::PureWeb::Ui::PureWebMouseEventArgs& mouseEvent);

    // Overrides
public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

    // Implementation
public:
    virtual ~CScribbleView();
    void OnExecuteClear(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless responses);
    void OnScribbleColorChanged(CSI::ValueChangedEventArgs args);


#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
    virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

private:
    // From CColor GDI Helper
    enum _namedColors 
    {
        aliceblue            = 0x00FFF8F0,   // RGB(0xF0, 0xF8, 0xFF) 
        antiquewhite         = 0x00D7EBFA,   // RGB(0xFA, 0xEB, 0xD7) 
        aqua                 = 0x00FFFF00,   // RGB(0x00, 0xFF, 0xFF) 
        aquamarine           = 0x00D4FF7F,   // RGB(0x7F, 0xFF, 0xD4) 
        azure                = 0x00FFFFF0,   // RGB(0xF0, 0xFF, 0xFF) 
        beige                = 0x00DCF5F5,   // RGB(0xF5, 0xF5, 0xDC) 
        bisque               = 0x00C4E4FF,   // RGB(0xFF, 0xE4, 0xC4) 
        black                = 0x00000000,   // RGB(0x00, 0x00, 0x00) 
        blanchedalmond       = 0x00CDEBFF,   // RGB(0xFF, 0xEB, 0xCD) 
        blue                 = 0x00FF0000,   // RGB(0x00, 0x00, 0xFF) 
        blueviolet           = 0x00E22B8A,   // RGB(0x8A, 0x2B, 0xE2) 
        brown                = 0x002A2AA5,   // RGB(0xA5, 0x2A, 0x2A) 
        burlywood            = 0x0087B8DE,   // RGB(0xDE, 0xB8, 0x87) 
        cadetblue            = 0x00A09E5F,   // RGB(0x5F, 0x9E, 0xA0) 
        chartreuse           = 0x0000FF7F,   // RGB(0x7F, 0xFF, 0x00) 
        chocolate            = 0x001E69D2,   // RGB(0xD2, 0x69, 0x1E) 
        coral                = 0x00507FFF,   // RGB(0xFF, 0x7F, 0x50) 
        cornflower           = 0x00ED9564,   // RGB(0x64, 0x95, 0xED) 
        cornsilk             = 0x00DCF8FF,   // RGB(0xFF, 0xF8, 0xDC) 
        crimson              = 0x003C14DC,   // RGB(0xDC, 0x14, 0x3C) 
        cyan                 = 0x00FFFF00,   // RGB(0x00, 0xFF, 0xFF) 
        darkblue             = 0x008B0000,   // RGB(0x00, 0x00, 0x8B) 
        darkcyan             = 0x008B8B00,   // RGB(0x00, 0x8B, 0x8B) 
        darkgoldenrod        = 0x000B86B8,   // RGB(0xB8, 0x86, 0x0B) 
        darkgray             = 0x00A9A9A9,   // RGB(0xA9, 0xA9, 0xA9) 
        darkgreen            = 0x00006400,   // RGB(0x00, 0x64, 0x00) 
        darkkhaki            = 0x006BB7BD,   // RGB(0xBD, 0xB7, 0x6B) 
        darkmagenta          = 0x008B008B,   // RGB(0x8B, 0x00, 0x8B) 
        darkolivegreen       = 0x002F6B55,   // RGB(0x55, 0x6B, 0x2F) 
        darkorange           = 0x00008CFF,   // RGB(0xFF, 0x8C, 0x00) 
        darkorchid           = 0x00CC3299,   // RGB(0x99, 0x32, 0xCC) 
        darkred              = 0x0000008B,   // RGB(0x8B, 0x00, 0x00) 
        darksalmon           = 0x007A96E9,   // RGB(0xE9, 0x96, 0x7A) 
        darkseagreen         = 0x008BBC8F,   // RGB(0x8F, 0xBC, 0x8B) 
        darkslateblue        = 0x008B3D48,   // RGB(0x48, 0x3D, 0x8B) 
        darkslategray        = 0x004F4F2F,   // RGB(0x2F, 0x4F, 0x4F) 
        darkturquoise        = 0x00D1CE00,   // RGB(0x00, 0xCE, 0xD1) 
        darkviolet           = 0x00D30094,   // RGB(0x94, 0x00, 0xD3) 
        deeppink             = 0x009314FF,   // RGB(0xFF, 0x14, 0x93) 
        deepskyblue          = 0x00FFBF00,   // RGB(0x00, 0xBF, 0xFF) 
        dimgray              = 0x00696969,   // RGB(0x69, 0x69, 0x69) 
        dodgerblue           = 0x00FF901E,   // RGB(0x1E, 0x90, 0xFF) 
        firebrick            = 0x002222B2,   // RGB(0xB2, 0x22, 0x22) 
        floralwhite          = 0x00F0FAFF,   // RGB(0xFF, 0xFA, 0xF0) 
        forestgreen          = 0x00228B22,   // RGB(0x22, 0x8B, 0x22) 
        fuchsia              = 0x00FF00FF,   // RGB(0xFF, 0x00, 0xFF) 
        gainsboro            = 0x00DCDCDC,   // RGB(0xDC, 0xDC, 0xDC) 
        ghostwhite           = 0x00FFF8F8,   // RGB(0xF8, 0xF8, 0xFF) 
        gold                 = 0x0000D7FF,   // RGB(0xFF, 0xD7, 0x00) 
        goldenrod            = 0x0020A5DA,   // RGB(0xDA, 0xA5, 0x20) 
        gray                 = 0x00808080,   // RGB(0x80, 0x80, 0x80) 
        green                = 0x00008000,   // RGB(0x00, 0x80, 0x00) 
        greenyellow          = 0x002FFFAD,   // RGB(0xAD, 0xFF, 0x2F) 
        honeydew             = 0x00F0FFF0,   // RGB(0xF0, 0xFF, 0xF0) 
        hotpink              = 0x00B469FF,   // RGB(0xFF, 0x69, 0xB4) 
        indianred            = 0x005C5CCD,   // RGB(0xCD, 0x5C, 0x5C) 
        indigo               = 0x0082004B,   // RGB(0x4B, 0x00, 0x82) 
        ivory                = 0x00F0FFFF,   // RGB(0xFF, 0xFF, 0xF0) 
        khaki                = 0x008CE6F0,   // RGB(0xF0, 0xE6, 0x8C) 
        lavender             = 0x00FAE6E6,   // RGB(0xE6, 0xE6, 0xFA) 
        lavenderblush        = 0x00F5F0FF,   // RGB(0xFF, 0xF0, 0xF5) 
        lawngreen            = 0x0000FC7C,   // RGB(0x7C, 0xFC, 0x00) 
        lemonchiffon         = 0x00CDFAFF,   // RGB(0xFF, 0xFA, 0xCD) 
        lightblue            = 0x00E6D8AD,   // RGB(0xAD, 0xD8, 0xE6) 
        lightcoral           = 0x008080F0,   // RGB(0xF0, 0x80, 0x80) 
        lightcyan            = 0x00FFFFE0,   // RGB(0xE0, 0xFF, 0xFF) 
        lightgoldenrodyellow = 0x00D2FAFA,   // RGB(0xFA, 0xFA, 0xD2) 
        lightgreen           = 0x0090EE90,   // RGB(0x90, 0xEE, 0x90) 
        lightgrey            = 0x00D3D3D3,   // RGB(0xD3, 0xD3, 0xD3) 
        lightpink            = 0x00C1B6FF,   // RGB(0xFF, 0xB6, 0xC1) 
        lightsalmon          = 0x007AA0FF,   // RGB(0xFF, 0xA0, 0x7A) 
        lightseagreen        = 0x00AAB220,   // RGB(0x20, 0xB2, 0xAA) 
        lightskyblue         = 0x00FACE87,   // RGB(0x87, 0xCE, 0xFA) 
        lightslategray       = 0x00998877,   // RGB(0x77, 0x88, 0x99) 
        lightsteelblue       = 0x00DEC4B0,   // RGB(0xB0, 0xC4, 0xDE) 
        lightyellow          = 0x00E0FFFF,   // RGB(0xFF, 0xFF, 0xE0) 
        lime                 = 0x0000FF00,   // RGB(0x00, 0xFF, 0x00) 
        limegreen            = 0x0032CD32,   // RGB(0x32, 0xCD, 0x32) 
        linen                = 0x00E6F0FA,   // RGB(0xFA, 0xF0, 0xE6) 
        magenta              = 0x00FF00FF,   // RGB(0xFF, 0x00, 0xFF) 
        maroon               = 0x00000080,   // RGB(0x80, 0x00, 0x00) 
        mediumaquamarine     = 0x00AACD66,   // RGB(0x66, 0xCD, 0xAA) 
        mediumblue           = 0x00CD0000,   // RGB(0x00, 0x00, 0xCD) 
        mediumorchid         = 0x00D355BA,   // RGB(0xBA, 0x55, 0xD3) 
        mediumpurple         = 0x00DB7093,   // RGB(0x93, 0x70, 0xDB) 
        mediumseagreen       = 0x0071B33C,   // RGB(0x3C, 0xB3, 0x71) 
        mediumslateblue      = 0x00EE687B,   // RGB(0x7B, 0x68, 0xEE) 
        mediumspringgreen    = 0x009AFA00,   // RGB(0x00, 0xFA, 0x9A) 
        mediumturquoise      = 0x00CCD148,   // RGB(0x48, 0xD1, 0xCC) 
        mediumvioletred      = 0x008515C7,   // RGB(0xC7, 0x15, 0x85) 
        midnightblue         = 0x00701919,   // RGB(0x19, 0x19, 0x70) 
        mintcream            = 0x00FAFFF5,   // RGB(0xF5, 0xFF, 0xFA) 
        mistyrose            = 0x00E1E4FF,   // RGB(0xFF, 0xE4, 0xE1) 
        moccasin             = 0x00B5E4FF,   // RGB(0xFF, 0xE4, 0xB5) 
        navajowhite          = 0x00ADDEFF,   // RGB(0xFF, 0xDE, 0xAD) 
        navy                 = 0x00800000,   // RGB(0x00, 0x00, 0x80) 
        oldlace              = 0x00E6F5FD,   // RGB(0xFD, 0xF5, 0xE6) 
        olive                = 0x00008080,   // RGB(0x80, 0x80, 0x00) 
        olivedrab            = 0x00238E6B,   // RGB(0x6B, 0x8E, 0x23) 
        orange               = 0x0000A5FF,   // RGB(0xFF, 0xA5, 0x00) 
        orangered            = 0x000045FF,   // RGB(0xFF, 0x45, 0x00) 
        orchid               = 0x00D670DA,   // RGB(0xDA, 0x70, 0xD6) 
        palegoldenrod        = 0x00AAE8EE,   // RGB(0xEE, 0xE8, 0xAA) 
        palegreen            = 0x0098FB98,   // RGB(0x98, 0xFB, 0x98) 
        paleturquoise        = 0x00EEEEAF,   // RGB(0xAF, 0xEE, 0xEE) 
        palevioletred        = 0x009370DB,   // RGB(0xDB, 0x70, 0x93) 
        papayawhip           = 0x00D5EFFF,   // RGB(0xFF, 0xEF, 0xD5) 
        peachpuff            = 0x00B9DAFF,   // RGB(0xFF, 0xDA, 0xB9) 
        peru                 = 0x003F85CD,   // RGB(0xCD, 0x85, 0x3F) 
        pink                 = 0x00CBC0FF,   // RGB(0xFF, 0xC0, 0xCB) 
        plum                 = 0x00DDA0DD,   // RGB(0xDD, 0xA0, 0xDD) 
        powderblue           = 0x00E6E0B0,   // RGB(0xB0, 0xE0, 0xE6) 
        purple               = 0x00800080,   // RGB(0x80, 0x00, 0x80) 
        red                  = 0x000000FF,   // RGB(0xFF, 0x00, 0x00) 
        rosybrown            = 0x008F8FBC,   // RGB(0xBC, 0x8F, 0x8F) 
        royalblue            = 0x00E16941,   // RGB(0x41, 0x69, 0xE1) 
        saddlebrown          = 0x0013458B,   // RGB(0x8B, 0x45, 0x13) 
        salmon               = 0x007280FA,   // RGB(0xFA, 0x80, 0x72) 
        sandybrown           = 0x0060A4F4,   // RGB(0xF4, 0xA4, 0x60) 
        seagreen             = 0x00578B2E,   // RGB(0x2E, 0x8B, 0x57) 
        seashell             = 0x00EEF5FF,   // RGB(0xFF, 0xF5, 0xEE) 
        sienna               = 0x002D52A0,   // RGB(0xA0, 0x52, 0x2D) 
        silver               = 0x00C0C0C0,   // RGB(0xC0, 0xC0, 0xC0) 
        skyblue              = 0x00EBCE87,   // RGB(0x87, 0xCE, 0xEB) 
        slateblue            = 0x00CD5A6A,   // RGB(0x6A, 0x5A, 0xCD) 
        slategray            = 0x00908070,   // RGB(0x70, 0x80, 0x90) 
        snow                 = 0x00FAFAFF,   // RGB(0xFF, 0xFA, 0xFA) 
        springgreen          = 0x007FFF00,   // RGB(0x00, 0xFF, 0x7F) 
        steelblue            = 0x00B48246,   // RGB(0x46, 0x82, 0xB4) 
        tan                  = 0x008CB4D2,   // RGB(0xD2, 0xB4, 0x8C) 
        teal                 = 0x00808000,   // RGB(0x00, 0x80, 0x80) 
        thistle              = 0x00D8BFD8,   // RGB(0xD8, 0xBF, 0xD8) 
        tomato               = 0x004763FF,   // RGB(0xFF, 0x63, 0x47) 
        turquoise            = 0x00D0E040,   // RGB(0x40, 0xE0, 0xD0) 
        violet               = 0x00EE82EE,   // RGB(0xEE, 0x82, 0xEE) 
        wheat                = 0x00B3DEF5,   // RGB(0xF5, 0xDE, 0xB3) 
        white                = 0x00FFFFFF,   // RGB(0xFF, 0xFF, 0xFF) 
        whitesmoke           = 0x00F5F5F5,   // RGB(0xF5, 0xF5, 0xF5) 
        yellow               = 0x0000FFFF,   // RGB(0xFF, 0xFF, 0x00) 
        yellowgreen          = 0x0032CD9A,   // RGB(0x9A, 0xCD, 0x32) 
    };
    DEFINE_ENUM(NamedColors,_namedColors)
    {
        "antiquewhite",antiquewhite,
            "aquamarine",aquamarine,           
            "azure",azure,                
            "beige",beige,                
            "bisque",bisque,               
            "black",black,                
            "blanchedalmond",blanchedalmond,       
            "blue",blue,                 
            "blueviolet",blueviolet,           
            "brown",brown,                
            "burlywood",burlywood,            
            "cadetblue",cadetblue,            
            "chartreuse",chartreuse,           
            "chocolate",chocolate,            
            "coral",coral,                
            "cornflower",cornflower,           
            "cornsilk",cornsilk,             
            "crimson",crimson,              
            "cyan",cyan,                 
            "darkblue",darkblue,             
            "darkcyan",darkcyan,             
            "darkgoldenrod",darkgoldenrod,        
            "darkgray",darkgray,             
            "darkgreen",darkgreen,            
            "darkkhaki",darkkhaki,            
            "darkmagenta",darkmagenta,          
            "darkolivegreen",darkolivegreen,       
            "darkorange",darkorange,           
            "darkorchid",darkorchid,           
            "darkred",darkred,              
            "darksalmon",darksalmon,           
            "darkseagreen",darkseagreen,         
            "darkslateblue",darkslateblue,        
            "darkslategray",darkslategray,        
            "darkturquoise",darkturquoise,        
            "darkviolet",darkviolet,           
            "deeppink",deeppink,             
            "deepskyblue",deepskyblue,          
            "dimgray",dimgray,              
            "dodgerblue",dodgerblue,           
            "firebrick",firebrick,            
            "floralwhite",floralwhite,          
            "forestgreen",forestgreen,          
            "fuchsia",fuchsia,              
            "gainsboro",gainsboro,            
            "ghostwhite",ghostwhite,           
            "gold",gold,                 
            "goldenrod",goldenrod,            
            "gray",gray,                 
            "green",green,                
            "greenyellow",greenyellow,          
            "honeydew",honeydew,             
            "hotpink",hotpink,              
            "indianred",indianred,            
            "indigo",indigo,               
            "ivory",ivory,                
            "khaki",khaki,                
            "lavender",lavender,             
            "lavenderblush",lavenderblush,        
            "lawngreen",lawngreen,            
            "lemonchiffon",lemonchiffon,         
            "lightblue",lightblue,            
            "lightcoral",lightcoral,           
            "lightcyan",lightcyan,            
            "lightgoldenrodyellow",lightgoldenrodyellow, 
            "lightgreen",lightgreen,           
            "lightgrey",lightgrey,            
            "lightpink",lightpink,            
            "lightsalmon",lightsalmon,          
            "lightseagreen",lightseagreen,        
            "lightskyblue",lightskyblue,         
            "lightslategray",lightslategray,       
            "lightsteelblue",lightsteelblue,       
            "lightyellow",lightyellow,          
            "lime",lime,                 
            "limegreen",limegreen,            
            "linen",linen,                
            "magenta",magenta,              
            "maroon",maroon,               
            "mediumaquamarine",mediumaquamarine,     
            "mediumblue",mediumblue,           
            "mediumorchid",mediumorchid,         
            "mediumpurple",mediumpurple,         
            "mediumseagreen",mediumseagreen,       
            "mediumslateblue",mediumslateblue,      
            "mediumspringgreen",mediumspringgreen,    
            "mediumturquoise",mediumturquoise,      
            "mediumvioletred",mediumvioletred,      
            "midnightblue",midnightblue,         
            "mintcream",mintcream,            
            "mistyrose",mistyrose,            
            "moccasin",moccasin,             
            "navajowhite",navajowhite,          
            "navy",navy,                 
            "oldlace",oldlace,              
            "olive",olive,                
            "olivedrab",olivedrab,            
            "orange",orange,               
            "orangered",orangered,            
            "orchid",orchid,               
            "palegoldenrod",palegoldenrod,        
            "palegreen",palegreen,            
            "paleturquoise",paleturquoise,        
            "palevioletred",palevioletred,        
            "papayawhip",papayawhip,           
            "peachpuff",peachpuff,            
            "peru",peru,                 
            "pink",pink,                 
            "plum",plum,                 
            "powderblue",powderblue,           
            "purple",purple,               
            "red",red,                  
            "rosybrown",rosybrown,            
            "royalblue",royalblue,            
            "saddlebrown",saddlebrown,          
            "salmon",salmon,               
            "sandybrown",sandybrown,           
            "seagreen",seagreen,             
            "seashell",seashell,             
            "sienna",sienna,               
            "silver",silver,               
            "skyblue",skyblue,              
            "slateblue",slateblue,            
            "slategray",slategray,            
            "snow",snow,                 
            "springgreen",springgreen,          
            "steelblue",steelblue,            
            "tan",tan,                  
            "teal",teal,                 
            "thistle",thistle,              
            "tomato",tomato,               
            "turquoise",turquoise,            
            "violet",violet,               
            "wheat",wheat,                
            "white",white,                
            "whitesmoke",whitesmoke,           
            "yellow",yellow,               
            "yellowgreen",yellowgreen 
    }
    END_ENUM(NamedColors);

public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in ScribbleView.cpp
inline CScribbleDoc* CScribbleView::GetDocument() const
{ return reinterpret_cast<CScribbleDoc*>(m_pDocument); }
#endif

