# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'selectionwindow.ui'
#
# Created: Tue Apr 12 17:36:11 2016
#      by: PyQt4 UI code generator 4.10.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from ctypes import *
lib = cdll.LoadLibrary('./libmiddleware.so')
lib.TestFunction.argtypes = [c_char_p, c_int, c_int, c_int, c_char_p]
lib.TestFunction.restype = py_object

try:
    _fromUtf8 = QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QApplication.translate(context, text, disambig)

class Ui_resultsWindow(object):
	def setupUi(self, resultsWindow, results, product):
		self.ResultsWindow = resultsWindow
		self.Results = results
		self.Product = product
		resultsWindow.setObjectName(_fromUtf8("resultsWindow"))
		resultsWindow.resize(980, 540)
		resultsWindow.setMouseTracking(True)
		self.centralWidget = QWidget(resultsWindow)
		self.centralWidget.setObjectName(_fromUtf8("centralWidget"))
		self.titleLabel = QLabel(self.centralWidget)
		self.titleLabel.setGeometry(QRect(450, 10, 420, 35))
		font = QFont()
		font.setPointSize(24)
		font.setBold(True)
		font.setWeight(75)
		self.titleLabel.setFont(font)
		self.titleLabel.setObjectName(_fromUtf8("titleLabel"))
		self.productLabel = QLabel(self.centralWidget)
		self.productLabel.setGeometry(QRect(120, 40, 200, 35))
		font = QFont()
		font.setPointSize(24)
		font.setBold(True)
		font.setWeight(75)
		self.productLabel.setFont(font)
		self.productLabel.setObjectName(_fromUtf8("productLabel"))
		resultsWindow.setCentralWidget(self.centralWidget)
		self.menuBar = QMenuBar(resultsWindow)
		self.menuBar.setGeometry(QRect(0, 0, 980, 25))
		self.menuBar.setObjectName(_fromUtf8("menuBar"))
		resultsWindow.setMenuBar(self.menuBar)
		self.mainToolBar = QToolBar(resultsWindow)
		self.mainToolBar.setObjectName(_fromUtf8("mainToolBar"))
		resultsWindow.addToolBar(Qt.TopToolBarArea, self.mainToolBar)
		self.statusBar = QStatusBar(resultsWindow)
		self.statusBar.setObjectName(_fromUtf8("statusBar"))
		resultsWindow.setStatusBar(self.statusBar)

		self.retranslateUi(resultsWindow)
		QMetaObject.connectSlotsByName(resultsWindow)
		self.displayResults()
		self.displayProduct()

	def retranslateUi(self, resultsWindow):
		resultsWindow.setWindowTitle(_translate("resultsWindow", "Product Recommendations", None))
		self.titleLabel.setText(_translate("resultsWindow", "Product Recommendations", None))
		self.productLabel.setText(_translate("resultsWindow", "Product", None))

	def displayProduct(self):
		self.displayProductWidget = QWidget(self.centralWidget)
		self.displayProductWidget.setGeometry(QRect(20, 70, 400, 133))
		self.displayProductWidget.setObjectName(_fromUtf8("displayProductWidget"))
		self.displayProductLayout = QVBoxLayout()
		self.displayProductWidget.setLayout(self.displayProductLayout)
		displayProductLabel = QLabel()
		pixels = QPixmap(_fromUtf8(self.Product))
		scaledPixels = pixels.scaled(self.displayProductWidget.size(), Qt.KeepAspectRatio)
		displayProductLabel.setPixmap(scaledPixels)
		self.displayProductLayout.addWidget(displayProductLabel)
		self.displayProductWidget.show()

	def displayResults(self):
		self.displayImagesScroller = QScrollArea(self.centralWidget)
		self.displayImagesScroller.setGeometry(QRect(350, 70, 600, 400))
		self.displayImagesWidget = QWidget(self.displayImagesScroller)
		self.displayImagesWidget.setObjectName(_fromUtf8("displayImagesWidget"))
		self.displayImagesLayout = QGridLayout()
		self.displayImagesWidget.setLayout(self.displayImagesLayout)
		length = len(self.Results)
		for i in range(length):
			displayImageLabel = QLabel()
			pixels = QPixmap(_fromUtf8(self.Results[i]))
			scaledPixels = pixels.scaled(400, 133, Qt.KeepAspectRatio)
			displayImageLabel.setPixmap(scaledPixels)
			self.displayImagesLayout.addWidget(displayImageLabel, i/3, i%3)
		self.displayImagesWidget.show()
		self.displayImagesScroller.setWidget(self.displayImagesWidget)

class Ui_selectionWindow(object):
    def setupUi(self, selectionWindow):
        self.SelectionWindow = selectionWindow
        self.ColorPriority = 1
        self.TexturePriority = 1
        self.ShapePriority = 1
        self.ProductCategory = "Shoes"
        self.FileName = None
        selectionWindow.setObjectName(_fromUtf8("selectionWindow"))
        selectionWindow.resize(980, 540)
        selectionWindow.setMouseTracking(True)
        self.centralWidget = QWidget(selectionWindow)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))
        self.uploadImageButton = QPushButton(self.centralWidget)
        self.uploadImageButton.setGeometry(QRect(260, 420, 100, 30))
        self.uploadImageButton.setMouseTracking(True)
        self.uploadImageButton.setObjectName(_fromUtf8("uploadImageButton"))
        self.uploadImageButton.clicked.connect(self.uploadImageButtonClicked)
        self.uploadImageWidget = QWidget(self.centralWidget)
        self.uploadImageWidget.setGeometry(QRect(20, 50, 590, 340))
        self.uploadImageWidget.setObjectName(_fromUtf8("uploadImageWidget"))
        self.uploadImageLabel = None
        self.uploadImageLayout = QVBoxLayout()
        self.uploadImageWidget.setLayout(self.uploadImageLayout)
        self.colorSlider = QSlider(self.centralWidget)
        self.colorSlider.setGeometry(QRect(800, 50, 150, 30))
        self.colorSlider.setMouseTracking(True)
        self.colorSlider.setMinimum(1)
        self.colorSlider.setMaximum(3)
        self.colorSlider.setPageStep(1)
        self.colorSlider.setOrientation(Qt.Horizontal)
        self.colorSlider.setObjectName(_fromUtf8("colorSlider"))
        self.colorSlider.valueChanged.connect(self.changeColorPriority)
        self.textureSlider = QSlider(self.centralWidget)
        self.textureSlider.setGeometry(QRect(800, 90, 150, 30))
        self.textureSlider.setMouseTracking(True)
        self.textureSlider.setMinimum(1)
        self.textureSlider.setMaximum(3)
        self.textureSlider.setPageStep(1)
        self.textureSlider.setOrientation(Qt.Horizontal)
        self.textureSlider.setObjectName(_fromUtf8("textureSlider"))
        self.textureSlider.valueChanged.connect(self.changeTexturePriority)
        self.shapeSlider = QSlider(self.centralWidget)
        self.shapeSlider.setGeometry(QRect(800, 130, 150, 30))
        self.shapeSlider.setMouseTracking(True)
        self.shapeSlider.setMinimum(1)
        self.shapeSlider.setMaximum(3)
        self.shapeSlider.setPageStep(1)
        self.shapeSlider.setOrientation(Qt.Horizontal)
        self.shapeSlider.setObjectName(_fromUtf8("shapeSlider"))
        self.shapeSlider.valueChanged.connect(self.changeShapePriority)
        self.colorLabel = QLabel(self.centralWidget)
        self.colorLabel.setGeometry(QRect(750, 55, 40, 20))
        self.colorLabel.setObjectName(_fromUtf8("colorLabel"))
        self.textureLabel = QLabel(self.centralWidget)
        self.textureLabel.setGeometry(QRect(735, 95, 55, 20))
        self.textureLabel.setObjectName(_fromUtf8("textureLabel"))
        self.shapeLabel = QLabel(self.centralWidget)
        self.shapeLabel.setGeometry(QRect(750, 135, 40, 20))
        self.shapeLabel.setObjectName(_fromUtf8("shapeLabel"))
        self.featureLabel = QLabel(self.centralWidget)
        self.featureLabel.setGeometry(QRect(800, 10, 150, 20))
        font = QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.featureLabel.setFont(font)
        self.featureLabel.setObjectName(_fromUtf8("featureLabel"))
        self.titleLabel = QLabel(self.centralWidget)
        self.titleLabel.setGeometry(QRect(130, 10, 380, 35))
        font = QFont()
        font.setPointSize(24)
        font.setBold(True)
        font.setWeight(75)
        self.titleLabel.setFont(font)
        self.titleLabel.setObjectName(_fromUtf8("titleLabel"))
        self.searchButton = QPushButton(self.centralWidget)
        self.searchButton.setGeometry(QRect(800, 220, 100, 30))
        self.searchButton.setObjectName(_fromUtf8("searchButton"))
        self.searchButton.clicked.connect(self.searchButtonClicked)
        self.inputCategory = QComboBox(self.centralWidget)
        self.inputCategory.setGeometry(QRect(800, 175, 100, 30))
        self.inputCategory.setObjectName(_fromUtf8("inputCategory"))
        self.inputCategory.addItem("Shoes")
        self.inputCategory.addItem("Watches")
        self.inputCategory.addItem("NeckTies")
        self.inputCategory.currentIndexChanged.connect(self.changeProductCategory)
        self.categoryLabel = QLabel(self.centralWidget)
        self.categoryLabel.setGeometry(QRect(730, 180, 65, 20))
        self.categoryLabel.setObjectName(_fromUtf8("categoryLabel"))
        selectionWindow.setCentralWidget(self.centralWidget)
        self.menuBar = QMenuBar(selectionWindow)
        self.menuBar.setGeometry(QRect(0, 0, 980, 25))
        self.menuBar.setObjectName(_fromUtf8("menuBar"))
        selectionWindow.setMenuBar(self.menuBar)
        self.mainToolBar = QToolBar(selectionWindow)
        self.mainToolBar.setObjectName(_fromUtf8("mainToolBar"))
        selectionWindow.addToolBar(Qt.TopToolBarArea, self.mainToolBar)
        self.statusBar = QStatusBar(selectionWindow)
        self.statusBar.setObjectName(_fromUtf8("statusBar"))
        selectionWindow.setStatusBar(self.statusBar)

        self.retranslateUi(selectionWindow)
        QMetaObject.connectSlotsByName(selectionWindow)

    def retranslateUi(self, selectionWindow):
        selectionWindow.setWindowTitle(_translate("selectionWindow", "AdvancedPotionMaking", None))
        self.uploadImageButton.setText(_translate("selectionWindow", "Upload Image", None))
        self.colorLabel.setText(_translate("selectionWindow", "Color", None))
        self.textureLabel.setText(_translate("selectionWindow", "Texture", None))
        self.shapeLabel.setText(_translate("selectionWindow", "Shape", None))
        self.featureLabel.setText(_translate("selectionWindow", "Feature Preference", None))
        self.titleLabel.setText(_translate("selectionWindow", "Advanced Potion Making", None))
        self.searchButton.setText(_translate("selectionWindow", "Search", None))
        self.categoryLabel.setText(_translate("selectionWindow", "Category", None))

    def uploadImageButtonClicked(self):
        self.FileName = QFileDialog.getOpenFileName()
        if self.uploadImageLabel is not None:
            self.uploadImageLabel.setParent(None)
        else:
            self.uploadImageWidget.show()
        self.uploadImageLabel = QLabel()
        pixels = QPixmap(_fromUtf8(self.FileName))
        scaledPixels = pixels.scaled(self.uploadImageWidget.size(), Qt.KeepAspectRatio)
        self.uploadImageLabel.setPixmap(scaledPixels)
        self.uploadImageLayout.addWidget(self.uploadImageLabel)
        self.uploadImageWidget.update()

    def searchButtonClicked(self):
        results = self.getSearchResults()
        resultsWindow = QMainWindow(self.SelectionWindow)
        resultsUI = Ui_resultsWindow()
        resultsUI.setupUi(resultsWindow, results, self.FileName)
        resultsWindow.show()

    def changeColorPriority(self):
        self.ColorPriority = self.colorSlider.value();

    def changeTexturePriority(self):
        self.TexturePriority = self.textureSlider.value();

    def changeShapePriority(self):
        self.ShapePriority = self.shapeSlider.value();

    def changeProductCategory(self):
        self.ProductCategory = self.inputCategory.currentText()

    def getSearchResults(self):
        fileName = create_string_buffer(str(self.FileName))
        productCategory = create_string_buffer(str(self.ProductCategory))
        results = lib.TestFunction(fileName, self.ColorPriority, self.TexturePriority, self.ShapePriority, productCategory)
        return results

if __name__ == "__main__":
    import sys
    app = QApplication(sys.argv)
    selectionWindow = QMainWindow()
    selectionUI = Ui_selectionWindow()
    selectionUI.setupUi(selectionWindow)
    selectionWindow.show()
    sys.exit(app.exec_())

