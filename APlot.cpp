#include "APlot.h"

#include <algorithm>
#include <cassert>

using namespace std;

namespace APLOT {
	const int numOfColors = 10;
	const char *plotColour[numOfColors] = { "green", "blue", "black", "orange", "cyan", "violet", "grey", "purple", "magenta", "light-red" };

	void basePlot::clear()
	{
		m_numOfPlots = 0;
	}

	const char * basePlot::m_plotColor() const
	{
		return plotColour[(m_numOfPlots - 1) % numOfColors];
	}

	gpcNewAddGraphMode basePlot::m_addMode() const
	{
		return m_numOfPlots == 1 ? GPC_NEW : GPC_ADD;
	}

	basePlot::~basePlot()
	{
		gpc_close(m_plot);
	}

	plot2d::plot2d(const char *plotTitle,       // Plot title
		const char *xLabel,                                 // X axis label
		const char *yLabel,                                 // Y axis label
		const double scalingMode,                           // Scaling mode
		const gpcPlotSignMode signMode,                // Sign mode - signed, positive, negative
		const gpcMultiFastMode multiFastMode,          // Multiplot / fast plot mode
		const gpcKeyMode keyMode)
	{
		m_plot = gpc_init_2d(plotTitle, xLabel, yLabel, scalingMode, signMode, multiFastMode, keyMode);
	}

	int plot2d::m_plot2d(const double *pData, const int graphLength, const double xMin, const double xMax, const char * pDataName, const char * plotType, const char * pColour)
	{
		m_numOfPlots++;
		double _xmax = 0.0;
		if (xMin == xMax && xMin == 0.0) {
			_xmax = graphLength - 1;
		}

		return gpc_plot_2d(m_plot, pData, graphLength, pDataName,
			xMin, _xmax, plotType,
			pColour ? pColour : m_plotColor(),
			m_addMode());
	}

	plotxy::plotxy(const char * plotTitle, const char * xLabel, const char * yLabel, const double dimension, const gpcKeyMode keyMode)
	{
		m_plot = gpc_init_xy(plotTitle, xLabel, yLabel, dimension, keyMode);
	}

	int plotxy::plot(const ComplexRect_s * pData, const int graphLength, const char * pDataName, const char * plotType, const char * pColour)
	{
		m_numOfPlots++;
		return gpc_plot_xy(m_plot, pData, graphLength, pDataName, plotType, 
			pColour ? pColour : m_plotColor(),
			m_addMode());
	}

	plotpz::plotpz(const char * plotTitle, const double dimension, const gpcKeyMode keyMode)
	{
		m_plot = gpc_init_pz(plotTitle, dimension, keyMode);
	}

	int plotpz::plot(const ComplexRect_s * pData, const int graphLength, const char * pDataName, const gpcPoleZeroMode poleZeroMode)
	{
		m_numOfPlots++;
		return gpc_plot_pz(m_plot, pData, graphLength, pDataName,
			poleZeroMode, m_addMode());
	}
}

namespace BPLOT {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_MSC_VER)
#include <Windows.h>
#define GNUPLOT_CMD "gnuplot"           
#define popen _popen
#define pclose _pclose
#else                               // Use GNU compiler
#include <unistd.h>
#include <time.h>
#if (GPC_DEBUG == 1)
#define GNUPLOT_CMD "tee debug.log | gnuplot -persist"    // Do not pipe the text output to null so that it can be used for debugging
	// #define GNUPLOT_CMD "gnuplot > debug.log"        // Pipe the text output to debug.log for debugging
#else
#define GNUPLOT_CMD "gnuplot > /dev/nul 2>&1"      // Pipe the text output to null for higher performance
#endif
#define mssleep(u) usleep(u*1000)
#endif

	class basePlot
	{
	public:
		basePlot();
		~basePlot();

	private:

	};

	basePlot::basePlot()
	{
	}

	basePlot::~basePlot()
	{
	}



}