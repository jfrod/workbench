#ifndef __MAP_SETTINGS_PALETTE_COLOR_MAPPING_WIDGET_H_
#define __MAP_SETTINGS_PALETTE_COLOR_MAPPING_WIDGET_H_

/*LICENSE_START*/
/*
 *  Copyright (C) 2014  Washington University School of Medicine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*LICENSE_END*/
#include <set>
#include <stdint.h> // needed by windows
#include <QWidget>
#include <qwt_plot.h>

class QAbstractButton;
class QCheckBox;
class QDoubleSpinBox;
class QComboBox;
class QLabel;
class QLayout;
class QPushButton;
class QRadioButton;
class QSpinBox;
class QwtPlot;


namespace caret {
    
    class CaretMappableDataFile;
    class EnumComboBoxTemplate;
    class FastStatistics;
    class Histogram;
    class PaletteColorMapping;
    class WuQDoubleSlider;
    class WuQWidgetObjectGroup;
    class WuQwtPlot;
    
    class MapSettingsPaletteColorMappingWidget : public QWidget {
        Q_OBJECT
        
    public:
        MapSettingsPaletteColorMappingWidget(QWidget* parent = 0);
        
        void updateEditor(CaretMappableDataFile* caretMappableDataFile,
                          const int32_t mapIndex);
                    
        virtual ~MapSettingsPaletteColorMappingWidget();

        void updateWidget();
        
    private:
        MapSettingsPaletteColorMappingWidget(const MapSettingsPaletteColorMappingWidget&);

        MapSettingsPaletteColorMappingWidget& operator=(const MapSettingsPaletteColorMappingWidget&);
        
    private slots:
        void thresholdLowSpinBoxValueChanged(double);
        void thresholdHighSpinBoxValueChanged(double);
        void thresholdLowSliderValueChanged(double);
        void thresholdHighSliderValueChanged(double);
        void thresholdTypeChanged(int);
        void thresholdRangeModeChanged();
        void thresholdLinkCheckBoxToggled(bool);
        
        void scaleAutoPercentageNegativeMaximumValueChanged(double value);
        void scaleAutoPercentageNegativeMinimumValueChanged(double value);
        void scaleAutoPercentagePositiveMinimumValueChanged(double value);
        void scaleAutoPercentagePositiveMaximumValueChanged(double value);
        
        void scaleAutoAbsolutePercentageMinimumValueChanged(double value);
        void scaleAutoAbsolutePercentageMaximumValueChanged(double value);
        
        void scaleFixedNegativeMaximumValueChanged(double value);
        void scaleFixedNegativeMinimumValueChanged(double value);
        void scaleFixedPositiveMinimumValueChanged(double value);
        void scaleFixedPositiveMaximumValueChanged(double value);
        
        void histogramControlChanged();
        void histogramResetViewButtonClicked();
        void applyAndUpdate();
        void applySelections();
        
        void applyAllMapsCheckBoxStateChanged(bool);
        
        void applyToMultipleFilesPushbuttonClicked();
        
        void contextMenuDisplayRequested(QContextMenuEvent* event,
                                         float graphX,
                                         float graphY);
        
        void normalizationModeComboBoxActivated(int);
        
    private:
        void updateEditorInternal(CaretMappableDataFile* caretMappableDataFile,
                                 const int32_t mapIndex);
        

        QWidget* createPaletteSection();
        QWidget* createThresholdSection();
        void updateThresholdSection();
        QWidget* createHistogramSection();
        QWidget* createHistogramControlSection();
        QWidget* createNormalizationControlSection();
        QWidget* createDataOptionsSection();
        
        void updateNormalizationControlSection();
        
        void updateAfterThresholdValuesChanged(const float lowThreshold,
                                               const float highThreshold);
        
        void updateHistogramPlot();
        
        void updateColoringAndGraphics();
        
        void updateThresholdControlsMinimumMaximumRangeValues();
        
        void setLayoutSpacingAndMargins(QLayout* layout);
        
        const Histogram* getHistogram(const FastStatistics* statisticsForAll) const;
        
        PaletteColorMapping* paletteColorMapping;
        
        QComboBox* paletteNameComboBox;
        
        QCheckBox* applyAllMapsCheckBox;
        QPushButton* applyToMultipleFilesPushButton;
        
        QRadioButton* scaleAutoRadioButton;
        QRadioButton* scaleAutoAbsolutePercentageRadioButton;
        QRadioButton* scaleAutoPercentageRadioButton;
        QRadioButton* scaleFixedRadioButton;
        
        QDoubleSpinBox* scaleAutoPercentageNegativeMaximumSpinBox;
        QDoubleSpinBox* scaleAutoPercentageNegativeMinimumSpinBox;
        QDoubleSpinBox* scaleAutoPercentagePositiveMinimumSpinBox;
        QDoubleSpinBox* scaleAutoPercentagePositiveMaximumSpinBox;
        
        QDoubleSpinBox* scaleAutoAbsolutePercentageMinimumSpinBox;
        QDoubleSpinBox* scaleAutoAbsolutePercentageMaximumSpinBox;
        
        QDoubleSpinBox* scaleFixedNegativeMaximumSpinBox;
        QDoubleSpinBox* scaleFixedNegativeMinimumSpinBox;
        QDoubleSpinBox* scaleFixedPositiveMinimumSpinBox;
        QDoubleSpinBox* scaleFixedPositiveMaximumSpinBox;
        
        QCheckBox* displayModePositiveCheckBox;
        QCheckBox* displayModeZeroCheckBox;
        QCheckBox* displayModeNegativeCheckBox;
        
        QCheckBox* interpolateColorsCheckBox;
        
        QComboBox* thresholdTypeComboBox;
        
        WuQDoubleSlider* thresholdLowSlider;
        WuQDoubleSlider* thresholdHighSlider;

        WuQWidgetObjectGroup* thresholdAdjustmentWidgetGroup;
        
        QDoubleSpinBox* thresholdLowSpinBox;
        QDoubleSpinBox* thresholdHighSpinBox;
        bool allowUpdateOfThresholdLowSpinBox;
        bool allowUpdateOfThresholdHighSpinBox;
        
        QRadioButton* thresholdShowInsideRadioButton;
        QRadioButton* thresholdShowOutsideRadioButton;
        
        EnumComboBoxTemplate* thresholdRangeModeComboBox;
        
        QCheckBox* thresholdLinkCheckBox;

        WuQwtPlot* thresholdPlot;
        
        QLabel* statisticsMinimumValueLabel;
        QLabel* statisticsMaximumValueLabel;
        QLabel* statisticsMeanValueLabel;
        QLabel* statisticsStandardDeviationLabel;
        
        QComboBox* m_normalizationModeComboBox;
        
        QRadioButton* histogramAllRadioButton;
        QRadioButton* histogramMatchPaletteRadioButton;
        
        QCheckBox* histogramUsePaletteColors;
        
        bool isHistogramColored;
        
        CaretMappableDataFile* caretMappableDataFile;
        CaretMappableDataFile* m_previousCaretMappableDataFile;
        
        int32_t mapFileIndex;
        
        std::set<CaretMappableDataFile*> previousApplyPaletteToMapFilesSelected;
        
        WuQWidgetObjectGroup* paletteWidgetGroup;
        WuQWidgetObjectGroup* thresholdWidgetGroup;
        
        
    };
    
#ifdef __MAP_SETTINGS_PALETTE_COLOR_MAPPING_WIDGET_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __MAP_SETTINGS_PALETTE_COLOR_MAPPING_WIDGET_DECLARE__

} // namespace
#endif  //__MAP_SETTINGS_PALETTE_COLOR_MAPPING_WIDGET_H_
