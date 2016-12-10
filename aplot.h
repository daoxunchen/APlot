#ifndef PLOT_H
#define PLOT_H

#include "gnuplot_c.h"

#include <vector>

namespace APLOT {
	class basePlot
	{
	public:
		~basePlot();
		void clear();
	protected:
		int m_numOfPlots = 0;
		h_GPC_Plot *m_plot = nullptr;

		gpcNewAddGraphMode m_addMode() const;
		const char* m_plotColor() const;
	};

	class plot2d :public basePlot
	{
	public:
		plot2d(const char *plotTitle = "figure",	// Plot title
			const char *xLabel = "x",				// X axis label
			const char *yLabel = "y",				// Y axis label
			const double scalingMode = GPC_AUTO_SCALE,		// Scaling mode
			const gpcPlotSignMode signMode = GPC_SIGNED,	// Sign mode - signed, positive, negative
			const gpcMultiFastMode multiFastMode = GPC_MULTIPLOT,	// Multiplot / fast plot mode
			const gpcKeyMode keyMode = GPC_KEY_ENABLE);	// Legend / key mode

		template<typename T>
		int plot(const T *pData,	// Dataset pointer
			const int graphLength,		// Dataset length		
			const double xMin = 0.0,			// Minimum X value
			const double xMax = 0.0,			// Maximum X value
			const char *pDataName = "data",	// Dataset title
			const char *plotType = "linespoints",	// Plot type - "lines", "points", "impulses", "linespoints"
			const char *pColour = nullptr)	// Colour - Use gnuplot> show colornames to see available colours
		{
			std::vector<double> dat(graphLength);
			copy(pData, pData + graphLength, dat.begin());
			return m_plot2d(dat.data(), graphLength, xMin, xMax, pDataName, plotType, pColour);
		}
		template<>
		int plot(const double *pData,	// Dataset pointer
			const int graphLength,		// Dataset length		
			const double xMin,			// Minimum X value
			const double xMax,			// Maximum X value
			const char *pDataName,	// Dataset title
			const char *plotType,	// Plot type - "lines", "points", "impulses", "linespoints"
			const char *pColour)	// Colour - Use gnuplot> show colornames to see available colours
		{
			return m_plot2d(pData, graphLength, xMin, xMax, pDataName, plotType, pColour);
		}

		template<typename T>
		int plot(const std::vector<T> data,
			const double xMin = 0.0,			// Minimum X value
			const double xMax = 0.0,			// Maximum X value
			const char *pDataName = "data",	// Dataset title
			const char *plotType = "linespoints",	// Plot type - "lines", "points", "impulses", "linespoints"
			const char *pColour = nullptr)	// Colour - Use gnuplot> show colornames to see available colours
		{
			std::vector<double> dat(data.size());
			copy(data.cbegin(), data.cend(), dat.begin());
			return m_plot2d(dat.data(), dat.size(), xMin, xMax, pDataName, plotType, pColour);
		}

		template<>
		int plot(const std::vector<double> data,
			const double xMin,			// Minimum X value
			const double xMax,			// Maximum X value
			const char *pDataName,	// Dataset title
			const char *plotType,	// Plot type - "lines", "points", "impulses", "linespoints"
			const char *pColour)	// Colour - Use gnuplot> show colornames to see available colours
		{			
			return m_plot2d(data.data(), data.size(), xMin, xMax, pDataName, plotType, pColour);
		}

	private:
		int m_plot2d(const double *pData,	// Dataset pointer
			const int graphLength,		// Dataset length		
			const double xMin,			// Minimum X value
			const double xMax,			// Maximum X value
			const char *pDataName,	// Dataset title
			const char *plotType,	// Plot type - "lines", "points", "impulses", "linespoints"
			const char *pColour);	// Colour - Use gnuplot> show colornames to see available colours
	};

	class plotxy :public basePlot
	{
	public:
		plotxy(const char *plotTitle = "figure",	// Plot title
			const char *xLabel = "x",	// X axis label
			const char *yLabel = "y",	// Y axis label
			const double dimension = GPC_AUTO_SCALE,// Dimension - this is square
			const gpcKeyMode keyMode = GPC_KEY_ENABLE);// Legend / key mode
		int plot(const ComplexRect_s *pData,	// Dataset pointer
			const int graphLength,				// Dataset length
			const char *pDataName = "data",			// Dataset title
			const char *plotType = "linespoints",	// Plot type - "lines", "points", "impulses", "linespoints"
			const char *pColour = nullptr);			// Colour - Use gnuplot> show colornames to see available colours
	};

	class plotpz :public basePlot
	{
	public:
		plotpz(const char *plotTitle = "figure",		// Plot title
			const double dimension = GPC_AUTO_SCALE,	// Dimension - this is square
			const gpcKeyMode keyMode = GPC_KEY_ENABLE);	// Legend / key mode
		int plot(const ComplexRect_s *pData,	// Dataset pointer
			const int graphLength,				// Dataset length
			const char *pDataName = "data",		// Dataset title
			const gpcPoleZeroMode poleZeroMode = GPC_COMPLEX_POLE);// Pole-zero mode
	};
}

namespace BPLOT {

}

#endif