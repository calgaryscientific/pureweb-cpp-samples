//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//
#define _CRT_SECURE_NO_DEPRECATE

#include "Canvas.h"
#include "DDxCineView.h"
#include "CSI/PureWeb/StateManager/ImageListManager.h"
#include "jpeg_decoder.h"


using namespace CSI;
using namespace CSI::PureWeb;
using namespace CSI::PureWeb::Server;
using namespace CSI::PureWeb::Ui;
using namespace CSI::PureWeb::Server::Imaging;

static Log::Logger logger = Log::GetLogger<DDxCineView>(); 

DDxCineView::DDxCineView()
{
    ViewName = "DDx_CineView";	
}

void DDxCineView::SetClientSize(Size clientSize)
{    
}

Size DDxCineView::GetActualSize()
{    
	return Size(640,360);
}

void DDxCineView::RenderView(RenderTarget target)
{
	 Image image = target.RenderTargetImage();
    Canvas c(image);
	c.Clear(PureWebColor::FromKnownColor(PureWebKnownColor::DarkGray));
    for(int i  = 0; i < image.Width(); i = i + 20)
    {
		c.DrawVLine(PureWebColor::FromKnownColor(PureWebKnownColor::White), 2, 0, image.Height(), i);		
    }
}


void DDxCineView::PostMouseEvent(const PureWebMouseEventArgs& mouseEvent) 
{        
}

void DDxCineView::RenderDeferred()
{        
    StateManager::Instance()->ViewManager().RenderViewDeferred(ViewName);
}

void DDxCineView::OnExecuteGenerateCine(Guid sessionId, Typeless command, Typeless responses)
{
    logger.Debug.Format("GenerateCine");

	// Generate cinematic
	for (int i = 0; i <= 85; i++){
		FILE * pFile;
		long lSize;
		unsigned char * buffer;
		size_t result;
		
		char strBuff[128];
		sprintf_s(strBuff, "cineFrames\\frame_%d.jpg\0", i);		
		pFile = fopen (strBuff, "rb" );
		if (pFile==NULL) 
		{
			logger.Debug.Format("File error"); 
			exit (1);
		}
		fseek (pFile , 0 , SEEK_END);
		lSize = ftell (pFile);
		rewind (pFile);
		buffer = (unsigned char*) malloc (sizeof(char)*lSize);
		if (buffer == NULL) 
		{
			logger.Debug.Format("Memory error"); 
			exit (2);
		}
		result = fread (buffer,1,lSize,pFile);
		if (result != lSize) 
		{
			logger.Debug.Format("Reading error"); 
			exit (3);
		}
		 
		Jpeg::Decoder decoder(buffer, lSize);
		if (decoder.GetResult() != Jpeg::Decoder::OK)
	    {
			printf("Error decoding the input file\n");
			exit(4);
		}

		PureWeb::Image image(decoder.GetImage(), decoder.GetWidth(), decoder.GetHeight(), PureWeb::PixelFormat::Rgb24, PureWeb::ScanLineOrder::TopDown, 4);				
		CineFrame frame;
		frame.image = image.Copy();		

		frame.deltaT = CSI::TimeSpan(0,0,0,6);
		m_frameList.Add(frame);
		TypelessStateLock lock(StateManager::Instance()->LockAppState());
		lock.Element("FrameCount").SetValue(m_frameList.Count());

		CSI::String path(strBuff);
		logger.Debug.Format("Saving {0} as frame {1}", path, m_frameList.Count());
		
		fclose (pFile);
		free (buffer);
		buffer = NULL;
	}
								    

    CSI::Guid m_cinematicId = command["CinematicId"].As<Guid>();
    int encodingQuality = command["EncodingQuality"].ValueOr<int>(50);
    size_t maxFrameCount = command["MaxFrameCount"].ValueOr<size_t>(0);

    if (maxFrameCount == 0)
    {
        maxFrameCount = m_frameList.Count();
    }
    else
    {
        maxFrameCount = CSI::Math::Min<size_t>(maxFrameCount, m_frameList.Count());
    }

    ImageList cine;

    try
    {
        cine.Start(m_cinematicId, maxFrameCount, RemoteImageFormat::Jpeg, encodingQuality);

        m_frameEnumerator = m_frameList.GetEnumerator();

        // queue up next frame generation on UI thread
		logger.Debug.Format("Getting Cine: {0}", m_cinematicId.ToString());    
        CSI::Threading::UiDispatcher::BeginInvoke(CSI::Bind(this, &DDxCineView::GenerateNextCineFrame), m_cinematicId);
    }
    catch(...)
    {
        cine.Finish();   
        m_cinematicId = CSI::Guid::Empty();   
        throw;
    } 
	
}

void DDxCineView::GenerateNextCineFrame(CSI::Guid m_cinematicId)
{
	bool isLastFrame = false;
    CSI::ExceptionPtr pException;
    
    // Nothing to do here. This threading issue needs attention (ms).
    if (m_cinematicId == Guid::Empty())
        return;	
    
    ImageList cine(m_cinematicId);
    
    if (m_frameEnumerator.MoveNext())
    {
        try
        {
			CineFrame frame = m_frameEnumerator.Current();
			logger.Debug.Format("Adding Cine Frame to List {0}", cine.FrameCount());    
            Typeless params;			
            params["DeltaT"] = frame.deltaT;
			cine.AddFrame(frame.image, params);
            // queue up next frame generation on UI thread
            CSI::Threading::UiDispatcher::BeginInvoke(CSI::Bind(this, &DDxCineView::GenerateNextCineFrame), m_cinematicId);
        }
        catch_all(pException)
        {
            logger.Error.Format("An error occurred while generating a cinematic: {0}.", pException->StackTrace());
            isLastFrame = true;
        }        
    }
    else
    {
        isLastFrame = true;
    }

    if (cine.FrameCount() == cine.MaxFrameCount())
    {
        isLastFrame = true;
    }

    if (isLastFrame)
    {
        pException = NULL;
        try
        {
            cine.Finish();
        }
        catch_all(pException)
        {
            logger.Error.Format("An error occurred while finishing a cinematic: {0}.", pException->StackTrace());
        }
                
        // clean up
        m_cinematicId = CSI::Guid::Empty();
    }
}

void DDxCineView::OnExecuteDestroyCine(Guid sessionId, Typeless command, Typeless responses)
{    
    Guid cineID = command["CinematicId"].As<Guid>();
    int currentFrame = command["CurrentFrameIndex"].As<int>();

    ImageListManager::Instance().RemoveImageList(cineID);
    
    logger.Debug.Format("DestroyCine {0}", currentFrame);
}

