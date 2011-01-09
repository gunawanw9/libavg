# -*- coding: utf-8 -*-
# libavg - Media Playback Engine.
# Copyright (C) 2003-2008 Ulrich von Zadow
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# Current versions can be found at www.libavg.de
#

import libavg

g_log = libavg.Logger.get()


class Button(libavg.DivNode):
    STATE_DISABLED = 2
    STATE_UP       = 4
    STATE_DOWN     = 8
    STATE_OVER     = 16
    
    def __init__(self, upNode = None, downNode = None, disabledNode = None, 
                 pressHandler = None, clickHandler = None, **kwargs):
        libavg.DivNode.__init__(self, **kwargs)
        self.crop = False
        
        self.__upNode       = upNode
        self.__downNode     = downNode
        self.__disabledNode = disabledNode
        
        self.__defaultHandler = lambda event: None
        self.__customPressHandler = pressHandler if pressHandler else self.__defaultHandler
        self.__customClickHandler = clickHandler if clickHandler else self.__defaultHandler

        self.__capturedCursorIds = set()
        self.__overCursorIds = set()
        
        self.__isCheckable = False
        self.__isToggled = False
        
        self.__state = Button.STATE_UP
        
        if self.__upNode and self.__downNode:
            self.__setupNodes()
        self.__activateEventHandler()
        
    def setEventHandler(self, type, source, func):
        raise RuntimeError("Setting event handlers for buttons is not supported")
    
    def getUpNode(self):
        return self.__upNode
    
    def getDownNode(self):
        return self.__downNode
    
    def getDisabledNode(self):
        return self.__disabledNode
    
    def setNodes(self, upNode, downNode, disabledNode = None):
        self.__upNode = upNode
        self.__downNode = downNode
        self.__disabledNode = disabledNode
        self.__setupNodes()
        
    def setPressHandler(self, handler):
        self.__customPressHandler = handler
        
    def setClickHandler(self, handler):
        self.__customClickHandler = handler
    
    def setCheckable(self, val):
        self.__isCheckable = val
        
    def isCheckable(self):
        return self.__isCheckable
    
    def setChecked(self, val):
        self.__isToggled = val
        state = Button.STATE_DOWN if self.__isToggled else Button.STATE_UP
        self.__changeState(state)
    
    def isChecked(self):
        return bool(self.__hasBitState(Button.STATE_DOWN))
    
    def setEnabled(self, isEnabled):
        state = Button.STATE_DISABLED if not(isEnabled) else Button.STATE_UP
        self.__changeState(state)
        
        if self.__hasBitState(Button.STATE_DISABLED):
            self.__deactivateEventHandler()
        else:
            self.__activateEventHandler()
        
    def isEnabled(self):
        return not(self.__hasBitState(Button.STATE_DISABLED))
    
    def setDebug(self, debug):
        self.elementoutlinecolor = 'FF0000' if debug else ''
    
    def isDebug(self):
        return self.elementoutlinecolor != ''
    
    def __setupNodes(self):
        while self.getNumChildren() > 0:
            self.removeChild(self.getChild(0))
            
        self.appendChild(self.__upNode)
        self.appendChild(self.__downNode)

        if self.__disabledNode:
            self.appendChild(self.__disabledNode)
        
        self.__updateSize()
        self.__updateNodesVisibility()

    def __setupReleaseHandlerTemplateMethod(self, handler):
        libavg.DivNode.setEventHandler(self, libavg.CURSORUP, libavg.MOUSE | libavg.TOUCH, handler)

    def __setupPressHandlerTemplateMethod(self, handler):
        libavg.DivNode.setEventHandler(self, libavg.CURSORDOWN, libavg.MOUSE | libavg.TOUCH, handler)
     
    def __setupOverHandlerTemplateMethod(self, handler):
        libavg.DivNode.setEventHandler(self, libavg.CURSOROVER, libavg.MOUSE | libavg.TOUCH, handler)
    
    def __setupOutHandlerTemplateMethod(self, handler):
        libavg.DivNode.setEventHandler(self, libavg.CURSOROUT, libavg.MOUSE | libavg.TOUCH, handler)

    def __updateNodesVisibility(self):
        if self.__hasBitState(Button.STATE_UP):
            self.__setNodesVisibility(self.__upNode)

        elif self.__hasBitState(Button.STATE_DOWN):
            self.__setNodesVisibility(self.__downNode)

        elif self.__hasBitState(Button.STATE_DISABLED):
            self.__setNodesVisibility(self.__disabledNode)
        else:
            pass
        
    def __updateSize(self):
        self.size = self.__upNode.size
    
    def __setNodesVisibility(self, node):
        nodes = (self.__upNode, self.__downNode, self.__disabledNode)
        for element in nodes:
            if not element:
                continue
            
            element.active = False
            if element == node:
                element.active = True
            
    def __toggleBitState(self, state):
        self.__state = self.__state ^ state
    
    def __extractBitState(self, state):
        return self.__state & state
    
    def __hasBitState(self, state):
        return self.__state & state
    
    def __changeState(self, state):
        self.__setState(self.__extractBitState(Button.STATE_OVER) | state)
        self.__updateNodesVisibility()
    
    def __setState(self, state):
        self.__state = state
        
    def __captureCursor(self, id):
        self.__capturedCursorIds.add(id)
        self.setEventCapture(id)
    
    def __releaseCapturedCursor(self, id):
        self.__capturedCursorIds.remove(id)
        self.releaseEventCapture(id)
    
    def __isCursorCaptured(self, id):
        return id in self.__capturedCursorIds
    
    def __getNumberOfCapturedCursors(self):
        return len(self.__capturedCursorIds)

    def __hasCapturedCursor(self):
        return len(self.__capturedCursorIds) != 0
    
    def __pressHandlerTemplateMethod(self, event):
        if not self.__isCursorCaptured(event.cursorid):
            self.__captureCursor(event.cursorid)
        
        if event.cursorid not in self.__overCursorIds:
            self.__overCursorIds.add(event.cursorid)
        
        if self.__getNumberOfCapturedCursors() > 1:
            return
        
        self.__changeState(Button.STATE_DOWN)
        self.__customPressHandler(event)
        return True
    
    def __releaseHandlerTemplateMethod(self, event):
        numberOfCapturedCursors = self.__getNumberOfCapturedCursors()
        numberOfOverCursors = len(self.__overCursorIds)
        
        if self.__isCursorCaptured(event.cursorid):
            self.__releaseCapturedCursor(event.cursorid)

        if event.cursorid in self.__overCursorIds:
            self.__overCursorIds.remove(event.cursorid)
            
        if numberOfCapturedCursors > 1:
            return

        if  numberOfCapturedCursors == 0:
            return

        if  numberOfOverCursors == 0:
            return
        
        newState = Button.STATE_UP
        if self.isCheckable():
            self.__isToggled = not self.__isToggled
            if self.__isToggled:
                newState = Button.STATE_DOWN
            
        self.__changeState(newState)
        
        if self.__hasCapturedCursor():
            pass

        if len(self.__overCursorIds):
            pass
        
        self.__customClickHandler(event)
        return True
    
    def __overHandlerTemplateMethod(self, event):
        if event.cursorid not in self.__overCursorIds:
            self.__overCursorIds.add(event.cursorid)
            
        if self.__hasCapturedCursor() and len(self.__overCursorIds):
            self.__changeState(Button.STATE_DOWN)
            
        self.__toggleBitState(Button.STATE_OVER)
        return True
    
    def __outHandlerTemplateMethod(self, event):
        if event.cursorid in self.__overCursorIds:
            self.__overCursorIds.remove(event.cursorid)
        
        if self.__hasCapturedCursor() and not len(self.__overCursorIds):
            newState = Button.STATE_UP
            if self.isCheckable():
                if self.__isToggled:
                    newState = Button.STATE_DOWN
            self.__changeState(newState)
        
        self.__toggleBitState(Button.STATE_OVER)
        return True
    
    def __activateEventHandler(self):
        self.__setupPressHandlerTemplateMethod(self.__pressHandlerTemplateMethod)
        self.__setupReleaseHandlerTemplateMethod(self.__releaseHandlerTemplateMethod)
        self.__setupOverHandlerTemplateMethod(self.__overHandlerTemplateMethod)
        self.__setupOutHandlerTemplateMethod(self.__outHandlerTemplateMethod)
    
    def __deactivateEventHandler(self):
        for id in self.__capturedCursorIds:
            self.releaseEventCapture(id)
        self.__capturedCursorIds = set()
        self.__setupPressHandlerTemplateMethod(self.__defaultHandler)
        self.__setupReleaseHandlerTemplateMethod(self.__defaultHandler)
        self.__setupOverHandlerTemplateMethod(self.__defaultHandler)
        self.__setupOutHandlerTemplateMethod(self.__defaultHandler)

        
