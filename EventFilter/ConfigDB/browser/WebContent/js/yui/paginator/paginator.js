/*
Copyright (c) 2008, Yahoo! Inc. All rights reserved.
Code licensed under the BSD License:
http://developer.yahoo.net/yui/license.txt
version: 2.6.0
*/
/**
 * The Paginator widget provides a set of controls to navigate through paged
 * data.
 *
 * @module paginator
 * @uses YAHOO.util.EventProvider
 * @uses YAHOO.util.AttributeProvider
 */

/**
 * Instantiate a Paginator, passing a configuration object to the contructor.
 * The configuration object should contain the following properties:
 * <ul>
 *   <li>rowsPerPage : <em>n</em> (int)</li>
 *   <li>totalRecords : <em>n</em> (int or Paginator.VALUE_UNLIMITED)</li>
 *   <li>containers : <em>id | el | arr</em> (HTMLElement reference, its id, or an array of either)</li>
 * </ul>
 *
 * @namespace YAHOO.widget
 * @class Paginator
 * @constructor
 * @param config {Object} Object literal to set instance and ui component
 * configuration.
 */
YAHOO.widget.Paginator = function (config) {
    var UNLIMITED = YAHOO.widget.Paginator.VALUE_UNLIMITED,
        lang      = YAHOO.lang,
        attrib, initialPage, records, perPage;

    config = lang.isObject(config) ? config : {};

    this.initConfig();

    this.initEvents();

    // Set the basic config keys first
    this.set('rowsPerPage',config.rowsPerPage,true);
    if (lang.isNumber(config.totalRecords)) {
        this.set('totalRecords',config.totalRecords,true);
    }
    
    this.initUIComponents();

    // Update the other config values
    for (attrib in config) {
        if (lang.hasOwnProperty(config,attrib)) {
            this.set(attrib,config[attrib],true);
        }
    }

    // Calculate the initial record offset
    initialPage = this.get('initialPage');
    records     = this.get('totalRecords');
    perPage     = this.get('rowsPerPage');
    if (initialPage > 1 && perPage !== UNLIMITED) {
        var startIndex = (initialPage - 1) * perPage;
        if (records === UNLIMITED || startIndex < records) {
            this.set('recordOffset',startIndex,true);
        }
    }
};


// Static members
YAHOO.lang.augmentObject(YAHOO.widget.Paginator, {
    /**
     * Incrementing index used to give instances unique ids.
     * @static
     * @property id
     * @type number
     * @private
     */
    id : 0,

    /**
     * Base of id strings used for ui components.
     * @static
     * @property ID_BASE
     * @type string
     * @private
     */
    ID_BASE : 'yui-pg',

    /**
     * Used to identify unset, optional configurations, or used explicitly in
     * the case of totalRecords to indicate unlimited pagination.
     * @static
     * @property VALUE_UNLIMITED
     * @type number
     * @final
     */
    VALUE_UNLIMITED : -1,

    /**
     * Default template used by Paginator instances.  Update this if you want
     * all new Paginators to use a different default template.
     * @static
     * @property TEMPLATE_DEFAULT
     * @type string
     */
    TEMPLATE_DEFAULT : "{FirstPageLink} {PreviousPageLink} {PageLinks} {NextPageLink} {LastPageLink}",

    /**
     * Common alternate pagination format, including page links, links for
     * previous, next, first and last pages as well as a rows-per-page
     * dropdown.  Offered as a convenience.
     * @static
     * @property TEMPLATE_ROWS_PER_PAGE
     * @type string
     */
    TEMPLATE_ROWS_PER_PAGE : "{FirstPageLink} {PreviousPageLink} {PageLinks} {NextPageLink} {LastPageLink} {RowsPerPageDropdown}",

    /**
     * Storage object for UI Components
     * @static
     * @property ui
     */
    ui : {}

},true);


// Instance members and methods
YAHOO.widget.Paginator.prototype = {

    // Instance members

    /**
     * Array of nodes in which to render pagination controls.  This is set via
     * the &quot;containers&quot; attribute.
     * @property _containers
     * @type Array(HTMLElement)
     * @private
     */
    _containers : [],

    /**
     * Flag used to indicate multiple attributes are being updated via setState
     * @property _batch
     * @type boolean
     * @protected
     */
    _batch : false,

    /**
     * Used by setState to indicate when a page change has occurred
     * @property _pageChanged
     * @type boolean
     * @protected
     */
    _pageChanged : false,

    /**
     * Temporary state cache used by setState to keep track of the previous
     * state for eventual pageChange event firing
     * @property _state
     * @type Object
     * @protected
     */
    _state : null,


    // Instance methods

    /**
     * Initialize the Paginator's attributes (see YAHOO.util.Element class
     * AttributeProvider).
     * @method initConfig
     * @private
     */
    initConfig : function () {

        var UNLIMITED = YAHOO.widget.Paginator.VALUE_UNLIMITED,
            l         = YAHOO.lang;

        /**
         * REQUIRED. Number of records constituting a &quot;page&quot;
         * @attribute rowsPerPage
         * @type integer
         */
        this.setAttributeConfig('rowsPerPage', {
            value     : 0,
            validator : l.isNumber
        });

        /**
         * REQUIRED. Node references or ids of nodes in which to render the
         * pagination controls.
         * @attribute containers
         * @type {string|HTMLElement|Array(string|HTMLElement)}
         */
        this.setAttributeConfig('containers', {
            value     : null,
            validator : function (val) {
                if (!l.isArray(val)) {
                    val = [val];
                }
                for (var i = 0, len = val.length; i < len; ++i) {
                    if (l.isString(val[i]) || 
                        (l.isObject(val[i]) && val[i].nodeType === 1)) {
                        continue;
                    }
                    return false;
                }
                return true;
            },
            method : function (val) {
                val = YAHOO.util.Dom.get(val);
                if (!l.isArray(val)) {
                    val = [val];
                }
                this._containers = val;
            }
        });

        /**
         * Total number of records to paginate through
         * @attribute totalRecords
         * @type integer
         * @default 0
         */
        this.setAttributeConfig('totalRecords', {
            value     : 0,
            validator : l.isNumber
        });

        /**
         * Zero based index of the record considered first on the current page.
         * For page based interactions, don't modify this attribute directly;
         * use setPage(n).
         * @attribute recordOffset
         * @type integer
         * @default 0
         */
        this.setAttributeConfig('recordOffset', {
            value     : 0,
            validator : function (val) {
                var total = this.get('totalRecords');
                if (l.isNumber(val)) {
                    return total === UNLIMITED || total > val ||
                           (total === 0 && val === 0);
                }

                return false;
            }
        });

        /**
         * Page to display on initial paint
         * @attribute initialPage
         * @type integer
         * @default 1
         */
        this.setAttributeConfig('initialPage', {
            value     : 1,
            validator : l.isNumber
        });

        /**
         * Template used to render controls.  The string will be used as
         * innerHTML on all specified container nodes.  Bracketed keys
         * (e.g. {pageLinks}) in the string will be replaced with an instance
         * of the so named ui component.
         * @see Paginator.TEMPLATE_DEFAULT
         * @see Paginator.TEMPLATE_ROWS_PER_PAGE
         * @attribute template
         * @type string
         */
        this.setAttributeConfig('template', {
            value : YAHOO.widget.Paginator.TEMPLATE_DEFAULT,
            validator : l.isString
        });

        /**
         * Class assigned to the element(s) containing pagination controls.
         * @attribute containerClass
         * @type string
         * @default 'yui-pg-container'
         */
        this.setAttributeConfig('containerClass', {
            value : 'yui-pg-container',
            validator : l.isString
        });

        /**
         * Display pagination controls even when there is only one page.  Set
         * to false to forgo rendering and/or hide the containers when there
         * is only one page of data.  Note if you are using the rowsPerPage
         * dropdown ui component, visibility will be maintained as long as the
         * number of records exceeds the smallest page size.
         * @attribute alwaysVisible
         * @type boolean
         * @default true
         */
        this.setAttributeConfig('alwaysVisible', {
            value : true,
            validator : l.isBoolean
        });

        /**
         * Update the UI immediately upon interaction.  If false, changeRequest
         * subscribers or other external code will need to explicitly set the
         * new values in the paginator to trigger repaint.
         * @attribute updateOnChange
         * @type boolean
         * @default false
         */
        this.setAttributeConfig('updateOnChange', {
            value     : false,
            validator : l.isBoolean
        });



        // Read only attributes

        /**
         * Unique id assigned to this instance
         * @attribute id
         * @type integer
         * @final
         */
        this.setAttributeConfig('id', {
            value    : YAHOO.widget.Paginator.id++,
            readOnly : true
        });

        /**
         * Indicator of whether the DOM nodes have been initially created
         * @attribute rendered
         * @type boolean
         * @final
         */
        this.setAttributeConfig('rendered', {
            value    : false,
            readOnly : true
        });

    },

    /**
     * Initialize registered ui components onto this instance.
     * @method initUIComponents
     * @private
     */
    initUIComponents : function () {
        var ui = YAHOO.widget.Paginator.ui,
            name,UIComp;
        for (name in ui) {
            if (YAHOO.lang.hasOwnProperty(ui,name)) {
                UIComp = ui[name];
                if (YAHOO.lang.isObject(UIComp) &&
                    YAHOO.lang.isFunction(UIComp.init)) {
                    UIComp.init(this);
                }
            }
        }
    },

    /**
     * Initialize this instance's CustomEvents.
     * @method initEvents
     * @private
     */
    initEvents : function () {
        this.createEvent('recordOffsetChange');
        this.createEvent('totalRecordsChange');
        this.createEvent('rowsPerPageChange');
        this.createEvent('alwaysVisibleChange');

        /**
         * Event fired when the Paginator is initially rendered
         * @event render
         */
        this.createEvent('render');

        /**
         * Event fired when the Paginator is initially rendered
         * @event rendered
         * @deprecated use render event
         */
        this.createEvent('rendered'); // backward compatibility

        /**
         * Event fired when a change in pagination values is requested,
         * either by interacting with the various ui components or via the
         * setStartIndex(n) etc APIs.
         * Subscribers will receive the proposed state as the first parameter.
         * The proposed state object will contain the following keys:
         * <ul>
         *   <li>paginator - the Paginator instance</li>
         *   <li>page</li>
         *   <li>totalRecords</li>
         *   <li>recordOffset - index of the first record on the new page</li>
         *   <li>rowsPerPage</li>
         *   <li>records - array containing [start index, end index] for the records on the new page</li>
         *   <li>before - object literal with all these keys for the current state</li>
         * </ul>
         * @event changeRequest
         */
        this.createEvent('changeRequest');

        /**
         * Event fired when attribute changes have resulted in the calculated
         * current page changing.
         * @event pageChange
         */
        this.createEvent('pageChange');

        /**
         * Event that fires before the destroy event.
         * @event beforeDestroy
         */
        this.createEvent('beforeDestroy');

        /**
         * Event used to trigger cleanup of ui components
         * @event destroy
         */
        this.createEvent('destroy');

        this._selfSubscribe();
    },

    /**
     * Subscribes to instance attribute change events to automate certain
     * behaviors.
     * @method _selfSubscribe
     * @protected
     */
    _selfSubscribe : function () {
        // Listen for changes to totalRecords and alwaysVisible 
        this.subscribe('totalRecordsChange',this.updateVisibility,this,true);
        this.subscribe('alwaysVisibleChange',this.updateVisibility,this,true);

        // Fire the pageChange event when appropriate
        this.subscribe('totalRecordsChange',this._handleStateChange,this,true);
        this.subscribe('recordOffsetChange',this._handleStateChange,this,true);
        this.subscribe('rowsPerPageChange',this._handleStateChange,this,true);

        // Update recordOffset when totalRecords is reduced below
        this.subscribe('totalRecordsChange',this._syncRecordOffset,this,true);
    },

    /**
     * Sets recordOffset to the starting index of the previous page when
     * totalRecords is reduced below the current recordOffset.
     * @method _syncRecordOffset
     * @param e {Event} totalRecordsChange event
     * @protected
     */
    _syncRecordOffset : function (e) {
        var v = e.newValue,rpp,state;
        if (e.prevValue !== v) {
            if (v !== YAHOO.widget.Paginator.VALUE_UNLIMITED) {
                rpp = this.get('rowsPerPage');

                if (rpp && this.get('recordOffset') >= v) {
                    state = this.getState({
                        totalRecords : e.prevValue,
                        recordOffset : this.get('recordOffset')
                    });

                    this.set('recordOffset', state.before.recordOffset);
                    this._firePageChange(state);
                }
            }
        }
    },

    /**
     * Fires the pageChange event when the state attributes have changed in
     * such a way as to locate the current recordOffset on a new page.
     * @method _handleStateChange
     * @param e {Event} the attribute change event
     * @protected
     */
    _handleStateChange : function (e) {
        if (e.prevValue !== e.newValue) {
            var change = this._state || {},
                state;

            change[e.type.replace(/Change$/,'')] = e.prevValue;
            state = this.getState(change);

            if (state.page !== state.before.page) {
                if (this._batch) {
                    this._pageChanged = true;
                } else {
                    this._firePageChange(state);
                }
            }
        }
    },

    /**
     * Fires a pageChange event in the form of a standard attribute change
     * event with additional properties prevState and newState.
     * @method _firePageChange
     * @param state {Object} the result of getState(oldState)
     * @protected
     */
    _firePageChange : function (state) {
        if (YAHOO.lang.isObject(state)) {
            var current = state.before;
            delete state.before;
            this.fireEvent('pageChange',{
                type      : 'pageChange',
                prevValue : state.page,
                newValue  : current.page,
                prevState : state,
                newState  : current
            });
        }
    },

    /**
     * Render the pagination controls per the format attribute into the
     * specified container nodes.
     * @method render
     */
    render : function () {
        if (this.get('rendered')) {
            return;
        }

        // Forgo rendering if only one page and alwaysVisible is off
        var totalRecords = this.get('totalRecords');
        if (totalRecords !== YAHOO.widget.Paginator.VALUE_UNLIMITED &&
            totalRecords < this.get('rowsPerPage') &&
            !this.get('alwaysVisible')) {
            return;
        }

        var Dom            = YAHOO.util.Dom,
            template       = this.get('template'),
            containerClass = this.get('containerClass');

        // add marker spans to the template html to indicate drop zones
        // for ui components
        template = template.replace(/\{([a-z0-9_ \-]+)\}/gi,
            '<span class="yui-pg-ui $1"></span>');
        for (var i = 0, len = this._containers.length; i < len; ++i) {
            var c       = this._containers[i],
                // ex. yui-pg0-1 (first paginator, second container)
                id_base = YAHOO.widget.Paginator.ID_BASE + this.get('id') +
                          '-' + i;

            if (!c) {
                continue;
            }
            // Hide the container while its contents are rendered
            c.style.display = 'none';

            Dom.addClass(c,containerClass);

            // Place the template innerHTML
            c.innerHTML = template;

            // Replace each marker with the ui component's render() output
            var markers = Dom.getElementsByClassName('yui-pg-ui','span',c);

            for (var j = 0, jlen = markers.length; j < jlen; ++j) {
                var m      = markers[j],
                    mp     = m.parentNode,
                    name   = m.className.replace(/\s*yui-pg-ui\s+/g,''),
                    UIComp = YAHOO.widget.Paginator.ui[name];

                if (YAHOO.lang.isFunction(UIComp)) {
                    var comp = new UIComp(this);
                    if (YAHOO.lang.isFunction(comp.render)) {
                        mp.replaceChild(comp.render(id_base),m);
                    }
                }
            }

            // Show the container allowing page reflow
            c.style.display = '';
        }

        // Set render attribute manually to support its readOnly contract
        if (this._containers.length) {
            this.setAttributeConfig('rendered',{value:true});

            this.fireEvent('render',this.getState());
            // For backward compatibility
            this.fireEvent('rendered',this.getState());
        }
    },

    /**
     * Removes controls from the page and unhooks events.
     * @method destroy
     */
    destroy : function () {
        this.fireEvent('beforeDestroy');
        this.fireEvent('destroy');

        this.setAttributeConfig('rendered',{value:false});
    },

    /**
     * Hides the containers if there is only one page of data and attribute
     * alwaysVisible is false.  Conversely, it displays the containers if either
     * there is more than one page worth of data or alwaysVisible is turned on.
     * @method updateVisibility
     */
    updateVisibility : function (e) {
        var alwaysVisible = this.get('alwaysVisible');
        if (e.type === 'alwaysVisibleChange' || !alwaysVisible) {
            var totalRecords = this.get('totalRecords'),
                visible = true,
                rpp = this.get('rowsPerPage'),
                rppOptions = this.get('rowsPerPageOptions'),
                i,len;

            if (YAHOO.lang.isArray(rppOptions)) {
                for (i = 0, len = rppOptions.length; i < len; ++i) {
                    rpp = Math.min(rpp,rppOptions[i]);
                }
            }

            if (totalRecords !== YAHOO.widget.Paginator.VALUE_UNLIMITED &&
                totalRecords <= rpp) {
                visible = false;
            }

            visible = visible || alwaysVisible;

            for (i = 0, len = this._containers.length; i < len; ++i) {
                YAHOO.util.Dom.setStyle(this._containers[i],'display',
                    visible ? '' : 'none');
            }
        }
    },




    /**
     * Get the configured container nodes
     * @method getContainerNodes
     * @return {Array} array of HTMLElement nodes
     */
    getContainerNodes : function () {
        return this._containers;
    },

    /**
     * Get the total number of pages in the data set according to the current
     * rowsPerPage and totalRecords values.  If totalRecords is not set, or
     * set to YAHOO.widget.Paginator.VALUE_UNLIMITED, returns
     * YAHOO.widget.Paginator.VALUE_UNLIMITED.
     * @method getTotalPages
     * @return {number}
     */
    getTotalPages : function () {
        var records = this.get('totalRecords');
        var perPage = this.get('rowsPerPage');

        // rowsPerPage not set.  Can't calculate
        if (!perPage) {
            return null;
        }

        if (records === YAHOO.widget.Paginator.VALUE_UNLIMITED) {
            return YAHOO.widget.Paginator.VALUE_UNLIMITED;
        }

        return Math.ceil(records/perPage);
    },

    /**
     * Does the requested page have any records?
     * @method hasPage
     * @param page {number} the page in question
     * @return {boolean}
     */
    hasPage : function (page) {
        if (!YAHOO.lang.isNumber(page) || page < 1) {
            return false;
        }

        var totalPages = this.getTotalPages();

        return (totalPages === YAHOO.widget.Paginator.VALUE_UNLIMITED || totalPages >= page);
    },

    /**
     * Get the page number corresponding to the current record offset.
     * @method getCurrentPage
     * @return {number}
     */
    getCurrentPage : function () {
        var perPage = this.get('rowsPerPage');
        if (!perPage || !this.get('totalRecords')) {
            return 0;
        }
        return Math.floor(this.get('recordOffset') / perPage) + 1;
    },

    /**
     * Are there records on the next page?
     * @method hasNextPage
     * @return {boolean}
     */
    hasNextPage : function () {
        var currentPage = this.getCurrentPage(),
            totalPages  = this.getTotalPages();

        return currentPage && (totalPages === YAHOO.widget.Paginator.VALUE_UNLIMITED || currentPage < totalPages);
    },

    /**
     * Get the page number of the next page, or null if the current page is the
     * last page.
     * @method getNextPage
     * @return {number}
     */
    getNextPage : function () {
        return this.hasNextPage() ? this.getCurrentPage() + 1 : null;
    },

    /**
     * Is there a page before the current page?
     * @method hasPreviousPage
     * @return {boolean}
     */
    hasPreviousPage : function () {
        return (this.getCurrentPage() > 1);
    },

    /**
     * Get the page number of the previous page, or null if the current page
     * is the first page.
     * @method getPreviousPage
     * @return {number}
     */
    getPreviousPage : function () {
        return (this.hasPreviousPage() ? this.getCurrentPage() - 1 : 1);
    },

    /**
     * Get the start and end record indexes of the specified page.
     * @method getPageRecords
     * @param page {number} (optional) The page (current page if not specified)
     * @return {Array} [start_index, end_index]
     */
    getPageRecords : function (page) {
        if (!YAHOO.lang.isNumber(page)) {
            page = this.getCurrentPage();
        }

        var perPage = this.get('rowsPerPage'),
            records = this.get('totalRecords'),
            start, end;

        if (!page || !perPage) {
            return null;
        }

        start = (page - 1) * perPage;
        if (records !== YAHOO.widget.Paginator.VALUE_UNLIMITED) {
            if (start >= records) {
                return null;
            }
            end = Math.min(start + perPage, records) - 1;
        } else {
            end = start + perPage - 1;
        }

        return [start,end];
    },

    /**
     * Set the current page to the provided page number if possible.
     * @method setPage
     * @param newPage {number} the new page number
     * @param silent {boolean} whether to forcibly avoid firing the
     * changeRequest event
     */
    setPage : function (page,silent) {
        if (this.hasPage(page) && page !== this.getCurrentPage()) {
            if (this.get('updateOnChange') || silent) {
                this.set('recordOffset', (page - 1) * this.get('rowsPerPage'));
            } else {
                this.fireEvent('changeRequest',this.getState({'page':page}));
            }
        }
    },

    /**
     * Get the number of rows per page.
     * @method getRowsPerPage
     * @return {number} the current setting of the rowsPerPage attribute
     */
    getRowsPerPage : function () {
        return this.get('rowsPerPage');
    },

    /**
     * Set the number of rows per page.
     * @method setRowsPerPage
     * @param rpp {number} the new number of rows per page
     * @param silent {boolean} whether to forcibly avoid firing the
     * changeRequest event
     */
    setRowsPerPage : function (rpp,silent) {
        if (YAHOO.lang.isNumber(rpp) && rpp > 0 &&
            rpp !== this.get('rowsPerPage')) {
            if (this.get('updateOnChange') || silent) {
                this.set('rowsPerPage',rpp);
            } else {
                this.fireEvent('changeRequest',
                    this.getState({'rowsPerPage':rpp}));
            }
        }
    },

    /**
     * Get the total number of records.
     * @method getTotalRecords
     * @return {number} the current setting of totalRecords attribute
     */
    getTotalRecords : function () {
        return this.get('totalRecords');
    },

    /**
     * Set the total number of records.
     * @method setTotalRecords
     * @param total {number} the new total number of records
     * @param silent {boolean} whether to forcibly avoid firing the changeRequest event
     */
    setTotalRecords : function (total,silent) {
        if (YAHOO.lang.isNumber(total) && total >= 0 &&
            total !== this.get('totalRecords')) {
            if (this.get('updateOnChange') || silent) {
                this.set('totalRecords',total);
            } else {
                this.fireEvent('changeRequest',
                    this.getState({'totalRecords':total}));
            }
        }
    },

    /**
     * Get the index of the first record on the current page
     * @method getStartIndex
     * @return {number} the index of the first record on the current page
     */
    getStartIndex : function () {
        return this.get('recordOffset');
    },

    /**
     * Move the record offset to a new starting index.  This will likely cause
     * the calculated current page to change.  You should probably use setPage.
     * @method setStartIndex
     * @param offset {number} the new record offset
     * @param silent {boolean} whether to forcibly avoid firing the changeRequest event
     */
    setStartIndex : function (offset,silent) {
        if (YAHOO.lang.isNumber(offset) && offset >= 0 &&
            offset !== this.get('recordOffset')) {
            if (this.get('updateOnChange') || silent) {
                this.set('recordOffset',offset);
            } else {
                this.fireEvent('changeRequest',
                    this.getState({'recordOffset':offset}));
            }
        }
    },

    /**
     * Get an object literal describing the current state of the paginator.  If
     * an object literal of proposed values is passed, the proposed state will
     * be returned as an object literal with the following keys:
     * <ul>
     * <li>paginator - instance of the Paginator</li>
     * <li>page - number</li>
     * <li>totalRecords - number</li>
     * <li>recordOffset - number</li>
     * <li>rowsPerPage - number</li>
     * <li>records - [ start_index, end_index ]</li>
     * <li>before - (OPTIONAL) { state object literal for current state }</li>
     * </ul>
     * @method getState
     * @return {object}
     * @param changes {object} OPTIONAL object literal with proposed values
     * Supported change keys include:
     * <ul>
     * <li>rowsPerPage</li>
     * <li>totalRecords</li>
     * <li>recordOffset OR</li>
     * <li>page</li>
     * </ul>
     */
    getState : function (changes) {
        var UNLIMITED = YAHOO.widget.Paginator.VALUE_UNLIMITED,
            l         = YAHOO.lang,
            M = Math, min = M.min, max = M.max, floor = M.floor, ceil = M.ceil,
            currentState, state, offset;

        function normalizeOffset(offset,total,rpp) {
            if (offset <= 0 || total === 0) {
                return 0;
            }
            if (total === UNLIMITED || total > offset) {
                return offset - (offset % rpp);
            }
            return total - (total % rpp || rpp);
        }

        currentState = {
            paginator    : this,
            totalRecords : this.get('totalRecords'),
            rowsPerPage  : this.get('rowsPerPage'),
            records      : this.getPageRecords()
        };
        currentState.recordOffset = normalizeOffset(
                                        this.get('recordOffset'),
                                        currentState.totalRecords,
                                        currentState.rowsPerPage);
        currentState.page = ceil(currentState.recordOffset /
                                 currentState.rowsPerPage) + 1;

        if (!changes) {
            return currentState;
        }

        state = {
            paginator    : this,
            before       : currentState,

            rowsPerPage  : changes.rowsPerPage || currentState.rowsPerPage,
            totalRecords : (l.isNumber(changes.totalRecords) ?
                                max(changes.totalRecords,UNLIMITED) :
                                currentState.totalRecords)
        };

        if (state.totalRecords === 0) {
            state.recordOffset =
            state.page         = 0;
        } else {
            offset = l.isNumber(changes.page) ?
                        (changes.page - 1) * state.rowsPerPage :
                        l.isNumber(changes.recordOffset) ?
                            changes.recordOffset :
                            currentState.recordOffset;

            state.recordOffset = normalizeOffset(offset,
                                    state.totalRecords,
                                    state.rowsPerPage);

            state.page = ceil(state.recordOffset / state.rowsPerPage) + 1;
        }

        state.records = [ state.recordOffset,
                          state.recordOffset + state.rowsPerPage - 1 ];

        // limit upper index to totalRecords - 1
        if (state.totalRecords !== UNLIMITED &&
            state.recordOffset < state.totalRecords && state.records &&
            state.records[1] > state.totalRecords - 1) {
            state.records[1] = state.totalRecords - 1;
        }

        return state;
    },

    /**
     * Convenience method to facilitate setting state attributes rowsPerPage,
     * totalRecords, recordOffset in batch.  Also supports calculating
     * recordOffset from state.page if state.recordOffset is not provided.
     * Fires only a single pageChange event, if appropriate.
     * This will not fire a changeRequest event.
     * @method setState
     * @param state {Object} Object literal of attribute:value pairs to set
     */
    setState : function (state) {
        if (YAHOO.lang.isObject(state)) {
            // get flux state based on current state with before state as well
            this._state = this.getState({});

            // use just the state props from the input obj
            state = {
                page         : state.page,
                rowsPerPage  : state.rowsPerPage,
                totalRecords : state.totalRecords,
                recordOffset : state.recordOffset
            };

            // calculate recordOffset from page if recordOffset not specified.
            // not using lang.isNumber for support of numeric strings
            if (state.page && state.recordOffset === undefined) {
                state.recordOffset = (state.page - 1) *
                    (state.rowsPerPage || this.get('rowsPerPage'));
            }

            this._batch = true;
            this._pageChanged = false;

            for (var k in state) {
                if (state.hasOwnProperty(k)) {
                    this.set(k,state[k]);
                }
            }

            this._batch = false;
            
            if (this._pageChanged) {
                this._pageChanged = false;

                this._firePageChange(this.getState(this._state));
            }
        }
    }
};

YAHOO.lang.augmentProto(YAHOO.widget.Paginator, YAHOO.util.AttributeProvider);
(function () {

var Paginator = YAHOO.widget.Paginator,
    l         = YAHOO.lang;

/**
 * ui Component to generate the textual report of current pagination status.
 * E.g. "Now viewing page 1 of 13".
 *
 * @namespace YAHOO.widget.Paginator.ui
 * @class CurrentPageReport
 * @for YAHOO.widget.Paginator
 *
 * @constructor
 * @param p {Pagintor} Paginator instance to attach to
 */
Paginator.ui.CurrentPageReport = function (p) {
    this.paginator = p;

    p.createEvent('pageReportClassChange');
    p.createEvent('pageReportTemplateChange');

    p.subscribe('recordOffsetChange', this.update,this,true);
    p.subscribe('rowsPerPageChange', this.update,this,true);
    p.subscribe('totalRecordsChange',this.update,this,true);
    p.subscribe('pageReportTemplateChange', this.update,this,true);
    p.subscribe('destroy',this.destroy,this,true);

    //TODO: make this work
    p.subscribe('pageReportClassChange', this.update,this,true);
};

/**
 * Decorates Paginator instances with new attributes. Called during
 * Paginator instantiation.
 * @method init
 * @param p {Paginator} Paginator instance to decorate
 * @static
 */
Paginator.ui.CurrentPageReport.init = function (p) {

    /**
     * CSS class assigned to the span containing the info.
     * @attribute pageReportClass
     * @default 'yui-pg-current'
     */
    p.setAttributeConfig('pageReportClass', {
        value : 'yui-pg-current',
        validator : l.isString
    });

    /**
     * Used as innerHTML for the span.  Place holders in the form of {name}
     * will be replaced with the so named value from the key:value map
     * generated by the function held in the pageReportValueGenerator attribute.
     * @attribute pageReportTemplate
     * @default '({currentPage} of {totalPages})'
     * @see pageReportValueGenerator attribute
     */
    p.setAttributeConfig('pageReportTemplate', {
        value : '({currentPage} of {totalPages})',
        validator : l.isString
    });

    /**
     * Function to generate the value map used to populate the
     * pageReportTemplate.  The function is passed the Paginator instance as a
     * parameter.  The default function returns a map with the following keys:
     * <ul>
     * <li>currentPage</li>
     * <li>totalPages</li>
     * <li>startIndex</li>
     * <li>endIndex</li>
     * <li>startRecord</li>
     * <li>endRecord</li>
     * <li>totalRecords</li>
     * </ul>
     * @attribute pageReportValueGenarator
     */
    p.setAttributeConfig('pageReportValueGenerator', {
        value : function (paginator) {
            var curPage = paginator.getCurrentPage(),
                records = paginator.getPageRecords();

            return {
                'currentPage' : records ? curPage : 0,
                'totalPages'  : paginator.getTotalPages(),
                'startIndex'  : records ? records[0] : 0,
                'endIndex'    : records ? records[1] : 0,
                'startRecord' : records ? records[0] + 1 : 0,
                'endRecord'   : records ? records[1] + 1 : 0,
                'totalRecords': paginator.get('totalRecords')
            };
        },
        validator : l.isFunction
    });
};

/**
 * Replace place holders in a string with the named values found in an
 * object literal.
 * @static
 * @method sprintf
 * @param template {string} The content string containing place holders
 * @param values {object} The key:value pairs used to replace the place holders
 * @return {string}
 */
Paginator.ui.CurrentPageReport.sprintf = function (template, values) {
    return template.replace(/\{([\w\s\-]+)\}/g, function (x,key) {
            return (key in values) ? values[key] : '';
        });
};

Paginator.ui.CurrentPageReport.prototype = {

    /**
     * Span node containing the formatted info
     * @property span
     * @type HTMLElement
     * @private
     */
    span : null,


    /**
     * Generate the span containing info formatted per the pageReportTemplate
     * attribute.
     * @method render
     * @param id_base {string} used to create unique ids for generated nodes
     * @return {HTMLElement}
     */
    render : function (id_base) {
        this.span = document.createElement('span');
        this.span.id        = id_base + '-page-report';
        this.span.className = this.paginator.get('pageReportClass');
        this.update();
        
        return this.span;
    },
    
    /**
     * Regenerate the content of the span if appropriate. Calls
     * CurrentPageReport.sprintf with the value of the pageReportTemplate
     * attribute and the value map returned from pageReportValueGenerator
     * function.
     * @method update
     * @param e {CustomEvent} The calling change event
     */
    update : function (e) {
        if (e && e.prevValue === e.newValue) {
            return;
        }

        this.span.innerHTML = Paginator.ui.CurrentPageReport.sprintf(
            this.paginator.get('pageReportTemplate'),
            this.paginator.get('pageReportValueGenerator')(this.paginator));
    },

    /**
     * Removes the link/span node and clears event listeners
     * removal.
     * @method destroy
     * @private
     */
    destroy : function () {
        this.span.parentNode.removeChild(this.span);
        this.span = null;
    }

};

})();
(function () {

var Paginator = YAHOO.widget.Paginator,
    l         = YAHOO.lang;

/**
 * ui Component to generate the page links
 *
 * @namespace YAHOO.widget.Paginator.ui
 * @class PageLinks
 * @for YAHOO.widget.Paginator
 *
 * @constructor
 * @param p {Pagintor} Paginator instance to attach to
 */
Paginator.ui.PageLinks = function (p) {
    this.paginator = p;

    p.createEvent('pageLinkClassChange');
    p.createEvent('currentPageClassChange');
    p.createEvent('pageLinksContainerClassChange');
    p.createEvent('pageLinksChange');

    p.subscribe('recordOffsetChange',this.update,this,true);
    p.subscribe('rowsPerPageChange',this.update,this,true);
    p.subscribe('totalRecordsChange',this.update,this,true);
    p.subscribe('pageLinksChange',   this.rebuild,this,true);
    p.subscribe('pageLinkClassChange', this.rebuild,this,true);
    p.subscribe('currentPageClassChange', this.rebuild,this,true);
    p.subscribe('destroy',this.destroy,this,true);

    //TODO: Make this work
    p.subscribe('pageLinksContainerClassChange', this.rebuild,this,true);
};

/**
 * Decorates Paginator instances with new attributes. Called during
 * Paginator instantiation.
 * @method init
 * @param p {Paginator} Paginator instance to decorate
 * @static
 */
Paginator.ui.PageLinks.init = function (p) {

    /**
     * CSS class assigned to each page link/span.
     * @attribute pageLinkClass
     * @default 'yui-pg-page'
     */
    p.setAttributeConfig('pageLinkClass', {
        value : 'yui-pg-page',
        validator : l.isString
    });

    /**
     * CSS class assigned to the current page span.
     * @attribute currentPageClass
     * @default 'yui-pg-current-page'
     */
    p.setAttributeConfig('currentPageClass', {
        value : 'yui-pg-current-page',
        validator : l.isString
    });

    /**
     * CSS class assigned to the span containing the page links.
     * @attribute pageLinksContainerClass
     * @default 'yui-pg-pages'
     */
    p.setAttributeConfig('pageLinksContainerClass', {
        value : 'yui-pg-pages',
        validator : l.isString
    });

    /**
     * Maximum number of page links to display at one time.
     * @attribute pageLinks
     * @default 10
     */
    p.setAttributeConfig('pageLinks', {
        value : 10,
        validator : l.isNumber
    });

    /**
     * Function used generate the innerHTML for each page link/span.  The
     * function receives as parameters the page number and a reference to the
     * paginator object.
     * @attribute pageLabelBuilder
     * @default function (page, paginator) { return page; }
     */
    p.setAttributeConfig('pageLabelBuilder', {
        value : function (page, paginator) { return page; },
        validator : l.isFunction
    });
};

/**
 * Calculates start and end page numbers given a current page, attempting
 * to keep the current page in the middle
 * @static
 * @method calculateRange
 * @param {int} currentPage  The current page
 * @param {int} totalPages   (optional) Maximum number of pages
 * @param {int} numPages     (optional) Preferred number of pages in range
 * @return {Array} [start_page_number, end_page_number]
 */
Paginator.ui.PageLinks.calculateRange = function (currentPage,totalPages,numPages) {
    var UNLIMITED = Paginator.VALUE_UNLIMITED,
        start, end, delta;

    // Either has no pages, or unlimited pages.  Show none.
    if (!currentPage || numPages === 0 || totalPages === 0 ||
        (totalPages === UNLIMITED && numPages === UNLIMITED)) {
        return [0,-1];
    }

    // Limit requested pageLinks if there are fewer totalPages
    if (totalPages !== UNLIMITED) {
        numPages = numPages === UNLIMITED ?
                    totalPages :
                    Math.min(numPages,totalPages);
    }

    // Determine start and end, trying to keep current in the middle
    start = Math.max(1,Math.ceil(currentPage - (numPages/2)));
    if (totalPages === UNLIMITED) {
        end = start + numPages - 1;
    } else {
        end = Math.min(totalPages, start + numPages - 1);
    }

    // Adjust the start index when approaching the last page
    delta = numPages - (end - start + 1);
    start = Math.max(1, start - delta);

    return [start,end];
};


Paginator.ui.PageLinks.prototype = {

    /**
     * Current page
     * @property current
     * @type number
     * @private
     */
    current     : 0,

    /**
     * Span node containing the page links
     * @property container
     * @type HTMLElement
     * @private
     */
    container   : null,


    /**
     * Generate the nodes and return the container node containing page links
     * appropriate to the current pagination state.
     * @method render
     * @param id_base {string} used to create unique ids for generated nodes
     * @return {HTMLElement}
     */
    render : function (id_base) {
        var p = this.paginator;

        // Set up container
        this.container = document.createElement('span');
        this.container.id        = id_base + '-pages';
        this.container.className = p.get('pageLinksContainerClass');
        YAHOO.util.Event.on(this.container,'click',this.onClick,this,true);

        // Call update, flagging a need to rebuild
        this.update({newValue : null, rebuild : true});

        return this.container;
    },

    /**
     * Update the links if appropriate
     * @method update
     * @param e {CustomEvent} The calling change event
     */
    update : function (e) {
        if (e && e.prevValue === e.newValue) {
            return;
        }

        var p           = this.paginator,
            currentPage = p.getCurrentPage();

        // Replace content if there's been a change
        if (this.current !== currentPage || !currentPage || e.rebuild) {
            var labelBuilder = p.get('pageLabelBuilder'),
                range        = Paginator.ui.PageLinks.calculateRange(
                                currentPage,
                                p.getTotalPages(),
                                p.get('pageLinks')),
                start        = range[0],
                end          = range[1],
                content      = '',
                linkTemplate,i;

            linkTemplate = '<a href="#" class="' + p.get('pageLinkClass') +
                           '" page="';
            for (i = start; i <= end; ++i) {
                if (i === currentPage) {
                    content +=
                        '<span class="' + p.get('currentPageClass') + ' ' +
                                          p.get('pageLinkClass') + '">' +
                        labelBuilder(i,p) + '</span>';
                } else {
                    content +=
                        linkTemplate + i + '">' + labelBuilder(i,p) + '</a>';
                }
            }

            this.container.innerHTML = content;
        }
    },

    /**
     * Force a rebuild of the page links.
     * @method rebuild
     * @param e {CustomEvent} The calling change event
     */
    rebuild     : function (e) {
        e.rebuild = true;
        this.update(e);
    },

    /**
     * Removes the page links container node and clears event listeners
     * @method destroy
     * @private
     */
    destroy : function () {
        YAHOO.util.Event.purgeElement(this.container,true);
        this.container.parentNode.removeChild(this.container);
        this.container = null;
    },

    /**
     * Listener for the container's onclick event.  Looks for qualifying link
     * clicks, and pulls the page number from the link's page attribute.
     * Sends link's page attribute to the Paginator's setPage method.
     * @method onClick
     * @param e {DOMEvent} The click event
     */
    onClick : function (e) {
        var t = YAHOO.util.Event.getTarget(e);
        if (t && YAHOO.util.Dom.hasClass(t,
                        this.paginator.get('pageLinkClass'))) {

            YAHOO.util.Event.stopEvent(e);

            this.paginator.setPage(parseInt(t.getAttribute('page'),10));
        }
    }

};

})();
(function () {

var Paginator = YAHOO.widget.Paginator,
    l         = YAHOO.lang;

/**
 * ui Component to generate the link to jump to the first page.
 *
 * @namespace YAHOO.widget.Paginator.ui
 * @class FirstPageLink
 * @for YAHOO.widget.Paginator
 *
 * @constructor
 * @param p {Pagintor} Paginator instance to attach to
 */
Paginator.ui.FirstPageLink = function (p) {
    this.paginator = p;

    p.createEvent('firstPageLinkLabelChange');
    p.createEvent('firstPageLinkClassChange');

    p.subscribe('recordOffsetChange',this.update,this,true);
    p.subscribe('rowsPerPageChange',this.update,this,true);
    p.subscribe('totalRecordsChange',this.update,this,true);
    p.subscribe('destroy',this.destroy,this,true);

    // TODO: make this work
    p.subscribe('firstPageLinkLabelChange',this.update,this,true);
    p.subscribe('firstPageLinkClassChange',this.update,this,true);
};

/**
 * Decorates Paginator instances with new attributes. Called during
 * Paginator instantiation.
 * @method init
 * @param p {Paginator} Paginator instance to decorate
 * @static
 */
Paginator.ui.FirstPageLink.init = function (p) {

    /**
     * Used as innerHTML for the first page link/span.
     * @attribute firstPageLinkLabel
     * @default '&lt;&lt;&nbsp;first'
     */
    p.setAttributeConfig('firstPageLinkLabel', {
        value : '&lt;&lt;&nbsp;first',
        validator : l.isString
    });

    /**
     * CSS class assigned to the link/span
     * @attribute firstPageLinkClass
     * @default 'yui-pg-first'
     */
    p.setAttributeConfig('firstPageLinkClass', {
        value : 'yui-pg-first',
        validator : l.isString
    });
};

// Instance members and methods
Paginator.ui.FirstPageLink.prototype = {

    /**
     * The currently placed HTMLElement node
     * @property current
     * @type HTMLElement
     * @private
     */
    current   : null,

    /**
     * Link node
     * @property link
     * @type HTMLElement
     * @private
     */
    link      : null,

    /**
     * Span node (inactive link)
     * @property span
     * @type HTMLElement
     * @private
     */
    span      : null,

    /**
     * Generate the nodes and return the appropriate node given the current
     * pagination state.
     * @method render
     * @param id_base {string} used to create unique ids for generated nodes
     * @return {HTMLElement}
     */
    render : function (id_base) {
        var p     = this.paginator,
            c     = p.get('firstPageLinkClass'),
            label = p.get('firstPageLinkLabel');

        this.link     = document.createElement('a');
        this.span     = document.createElement('span');

        this.link.id        = id_base + '-first-link';
        this.link.href      = '#';
        this.link.className = c;
        this.link.innerHTML = label;
        YAHOO.util.Event.on(this.link,'click',this.onClick,this,true);

        this.span.id        = id_base + '-first-span';
        this.span.className = c;
        this.span.innerHTML = label;

        this.current = p.get('recordOffset') < 1 ? this.span : this.link;
        return this.current;
    },

    /**
     * Swap the link and span nodes if appropriate.
     * @method update
     * @param e {CustomEvent} The calling change event
     */
    update : function (e) {
        if (e && e.prevValue === e.newValue) {
            return;
        }

        var par = this.current ? this.current.parentNode : null;
        if (this.paginator.get('recordOffset') < 1) {
            if (par && this.current === this.link) {
                par.replaceChild(this.span,this.current);
                this.current = this.span;
            }
        } else {
            if (par && this.current === this.span) {
                par.replaceChild(this.link,this.current);
                this.current = this.link;
            }
        }
    },

    /**
     * Removes the link/span node and clears event listeners
     * removal.
     * @method destroy
     * @private
     */
    destroy : function () {
        YAHOO.util.Event.purgeElement(this.link);
        this.current.parentNode.removeChild(this.current);
        this.link = this.span = null;
    },

    /**
     * Listener for the link's onclick event.  Pass new value to setPage method.
     * @method onClick
     * @param e {DOMEvent} The click event
     */
    onClick : function (e) {
        YAHOO.util.Event.stopEvent(e);
        this.paginator.setPage(1);
    }
};

})();
(function () {

var Paginator = YAHOO.widget.Paginator,
    l         = YAHOO.lang;

/**
 * ui Component to generate the link to jump to the last page.
 *
 * @namespace YAHOO.widget.Paginator.ui
 * @class LastPageLink
 * @for YAHOO.widget.Paginator
 *
 * @constructor
 * @param p {Pagintor} Paginator instance to attach to
 */
Paginator.ui.LastPageLink = function (p) {
    this.paginator = p;

    p.createEvent('lastPageLinkLabelChange');
    p.createEvent('lastPageLinkClassChange');

    p.subscribe('recordOffsetChange',this.update,this,true);
    p.subscribe('rowsPerPageChange',this.update,this,true);
    p.subscribe('totalRecordsChange',this.update,this,true);
    p.subscribe('destroy',this.destroy,this,true);

    // TODO: make this work
    p.subscribe('lastPageLinkLabelChange',this.update,this,true);
    p.subscribe('lastPageLinkClassChange', this.update,this,true);
};

/**
 * Decorates Paginator instances with new attributes. Called during
 * Paginator instantiation.
 * @method init
 * @param paginator {Paginator} Paginator instance to decorate
 * @static
 */
Paginator.ui.LastPageLink.init = function (p) {

    /**
     * Used as innerHTML for the last page link/span.
     * @attribute lastPageLinkLabel
     * @default 'last&nbsp;&gt;&gt;'
     */
    p.setAttributeConfig('lastPageLinkLabel', {
        value : 'last&nbsp;&gt;&gt;',
        validator : l.isString
    });

    /**
     * CSS class assigned to the link/span
     * @attribute lastPageLinkClass
     * @default 'yui-pg-last'
     */
    p.setAttributeConfig('lastPageLinkClass', {
        value : 'yui-pg-last',
        validator : l.isString
    });
};

Paginator.ui.LastPageLink.prototype = {

    /**
     * Currently placed HTMLElement node
     * @property current
     * @type HTMLElement
     * @private
     */
    current   : null,

    /**
     * Link HTMLElement node
     * @property link
     * @type HTMLElement
     * @private
     */
    link      : null,

    /**
     * Span node (inactive link)
     * @property span
     * @type HTMLElement
     * @private
     */
    span      : null,

    /**
     * Empty place holder node for when the last page link is inappropriate to
     * display in any form (unlimited paging).
     * @property na
     * @type HTMLElement
     * @private
     */
    na        : null,


    /**
     * Generate the nodes and return the appropriate node given the current
     * pagination state.
     * @method render
     * @param id_base {string} used to create unique ids for generated nodes
     * @return {HTMLElement}
     */
    render : function (id_base) {
        var p     = this.paginator,
            c     = p.get('lastPageLinkClass'),
            label = p.get('lastPageLinkLabel'),
            last  = p.getTotalPages();

        this.link = document.createElement('a');
        this.span = document.createElement('span');
        this.na   = this.span.cloneNode(false);

        this.link.id        = id_base + '-last-link';
        this.link.href      = '#';
        this.link.className = c;
        this.link.innerHTML = label;
        YAHOO.util.Event.on(this.link,'click',this.onClick,this,true);

        this.span.id        = id_base + '-last-span';
        this.span.className = c;
        this.span.innerHTML = label;

        this.na.id = id_base + '-last-na';

        switch (last) {
            case Paginator.VALUE_UNLIMITED :
                    this.current = this.na; break;
            case p.getCurrentPage() :
                    this.current = this.span; break;
            default :
                    this.current = this.link;
        }

        return this.current;
    },

    /**
     * Swap the link, span, and na nodes if appropriate.
     * @method update
     * @param e {CustomEvent} The calling change event (ignored)
     */
    update : function (e) {
        if (e && e.prevValue === e.newValue) {
            return;
        }

        var par   = this.current ? this.current.parentNode : null,
            after = this.link;

        if (par) {
            switch (this.paginator.getTotalPages()) {
                case Paginator.VALUE_UNLIMITED :
                        after = this.na; break;
                case this.paginator.getCurrentPage() :
                        after = this.span; break;
            }

            if (this.current !== after) {
                par.replaceChild(after,this.current);
                this.current = after;
            }
        }
    },

    /**
     * Removes the link/span node and clears event listeners
     * @method destroy
     * @private
     */
    destroy : function () {
        YAHOO.util.Event.purgeElement(this.link);
        this.current.parentNode.removeChild(this.current);
        this.link = this.span = null;
    },

    /**
     * Listener for the link's onclick event.  Passes to setPage method.
     * @method onClick
     * @param e {DOMEvent} The click event
     */
    onClick : function (e) {
        YAHOO.util.Event.stopEvent(e);
        this.paginator.setPage(this.paginator.getTotalPages());
    }
};

})();
(function () {

var Paginator = YAHOO.widget.Paginator,
    l         = YAHOO.lang;

/**
 * ui Component to generate the link to jump to the next page.
 *
 * @namespace YAHOO.widget.Paginator.ui
 * @class NextPageLink
 * @for YAHOO.widget.Paginator
 *
 * @constructor
 * @param p {Pagintor} Paginator instance to attach to
 */
Paginator.ui.NextPageLink = function (p) {
    this.paginator = p;

    p.createEvent('nextPageLinkLabelChange');
    p.createEvent('nextPageLinkClassChange');

    p.subscribe('recordOffsetChange', this.update,this,true);
    p.subscribe('rowsPerPageChange', this.update,this,true);
    p.subscribe('totalRecordsChange', this.update,this,true);
    p.subscribe('destroy',this.destroy,this,true);

    // TODO: make this work
    p.subscribe('nextPageLinkLabelChange', this.update,this,true);
    p.subscribe('nextPageLinkClassChange', this.update,this,true);
};

/**
 * Decorates Paginator instances with new attributes. Called during
 * Paginator instantiation.
 * @method init
 * @param p {Paginator} Paginator instance to decorate
 * @static
 */
Paginator.ui.NextPageLink.init = function (p) {

    /**
     * Used as innerHTML for the next page link/span.
     * @attribute nextPageLinkLabel
     * @default 'next&nbsp;&gt;'
     */
    p.setAttributeConfig('nextPageLinkLabel', {
        value : 'next&nbsp;&gt;',
        validator : l.isString
    });

    /**
     * CSS class assigned to the link/span
     * @attribute nextPageLinkClass
     * @default 'yui-pg-next'
     */
    p.setAttributeConfig('nextPageLinkClass', {
        value : 'yui-pg-next',
        validator : l.isString
    });
};

Paginator.ui.NextPageLink.prototype = {

    /**
     * Currently placed HTMLElement node
     * @property current
     * @type HTMLElement
     * @private
     */
    current   : null,

    /**
     * Link node
     * @property link
     * @type HTMLElement
     * @private
     */
    link      : null,

    /**
     * Span node (inactive link)
     * @property span
     * @type HTMLElement
     * @private
     */
    span      : null,


    /**
     * Generate the nodes and return the appropriate node given the current
     * pagination state.
     * @method render
     * @param id_base {string} used to create unique ids for generated nodes
     * @return {HTMLElement}
     */
    render : function (id_base) {
        var p     = this.paginator,
            c     = p.get('nextPageLinkClass'),
            label = p.get('nextPageLinkLabel'),
            last  = p.getTotalPages();

        this.link     = document.createElement('a');
        this.span     = document.createElement('span');

        this.link.id        = id_base + '-next-link';
        this.link.href      = '#';
        this.link.className = c;
        this.link.innerHTML = label;
        YAHOO.util.Event.on(this.link,'click',this.onClick,this,true);

        this.span.id        = id_base + '-next-span';
        this.span.className = c;
        this.span.innerHTML = label;

        this.current = p.getCurrentPage() === last ? this.span : this.link;

        return this.current;
    },

    /**
     * Swap the link and span nodes if appropriate.
     * @method update
     * @param e {CustomEvent} The calling change event
     */
    update : function (e) {
        if (e && e.prevValue === e.newValue) {
            return;
        }

        var last = this.paginator.getTotalPages(),
            par  = this.current ? this.current.parentNode : null;

        if (this.paginator.getCurrentPage() !== last) {
            if (par && this.current === this.span) {
                par.replaceChild(this.link,this.current);
                this.current = this.link;
            }
        } else if (this.current === this.link) {
            if (par) {
                par.replaceChild(this.span,this.current);
                this.current = this.span;
            }
        }
    },

    /**
     * Removes the link/span node and clears event listeners
     * @method destroy
     * @private
     */
    destroy : function () {
        YAHOO.util.Event.purgeElement(this.link);
        this.current.parentNode.removeChild(this.current);
        this.link = this.span = null;
    },

    /**
     * Listener for the link's onclick event.  Passes to setPage method.
     * @method onClick
     * @param e {DOMEvent} The click event
     */
    onClick : function (e) {
        YAHOO.util.Event.stopEvent(e);
        this.paginator.setPage(this.paginator.getNextPage());
    }
};

})();
(function () {

var Paginator = YAHOO.widget.Paginator,
    l         = YAHOO.lang;

/**
 * ui Component to generate the link to jump to the previous page.
 *
 * @namespace YAHOO.widget.Paginator.ui
 * @class PreviousPageLink
 * @for YAHOO.widget.Paginator
 *
 * @constructor
 * @param p {Pagintor} Paginator instance to attach to
 */
Paginator.ui.PreviousPageLink = function (p) {
    this.paginator = p;

    p.createEvent('previousPageLinkLabelChange');
    p.createEvent('previousPageLinkClassChange');

    p.subscribe('recordOffsetChange',this.update,this,true);
    p.subscribe('rowsPerPageChange',this.update,this,true);
    p.subscribe('totalRecordsChange',this.update,this,true);
    p.subscribe('destroy',this.destroy,this,true);

    // TODO: make this work
    p.subscribe('previousPageLinkLabelChange',this.update,this,true);
    p.subscribe('previousPageLinkClassChange',this.update,this,true);
};

/**
 * Decorates Paginator instances with new attributes. Called during
 * Paginator instantiation.
 * @method init
 * @param p {Paginator} Paginator instance to decorate
 * @static
 */
Paginator.ui.PreviousPageLink.init = function (p) {

    /**
     * Used as innerHTML for the previous page link/span.
     * @attribute previousPageLinkLabel
     * @default '&lt;&nbsp;prev'
     */
    p.setAttributeConfig('previousPageLinkLabel', {
        value : '&lt;&nbsp;prev',
        validator : l.isString
    });

    /**
     * CSS class assigned to the link/span
     * @attribute previousPageLinkClass
     * @default 'yui-pg-previous'
     */
    p.setAttributeConfig('previousPageLinkClass', {
        value : 'yui-pg-previous',
        validator : l.isString
    });
};

Paginator.ui.PreviousPageLink.prototype = {

    /**
     * Currently placed HTMLElement node
     * @property current
     * @type HTMLElement
     * @private
     */
    current   : null,

    /**
     * Link node
     * @property link
     * @type HTMLElement
     * @private
     */
    link      : null,

    /**
     * Span node (inactive link)
     * @property span
     * @type HTMLElement
     * @private
     */
    span      : null,


    /**
     * Generate the nodes and return the appropriate node given the current
     * pagination state.
     * @method render
     * @param id_base {string} used to create unique ids for generated nodes
     * @return {HTMLElement}
     */
    render : function (id_base) {
        var p     = this.paginator,
            c     = p.get('previousPageLinkClass'),
            label = p.get('previousPageLinkLabel');

        this.link     = document.createElement('a');
        this.span     = document.createElement('span');

        this.link.id        = id_base + '-prev-link';
        this.link.href      = '#';
        this.link.className = c;
        this.link.innerHTML = label;
        YAHOO.util.Event.on(this.link,'click',this.onClick,this,true);

        this.span.id        = id_base + '-prev-span';
        this.span.className = c;
        this.span.innerHTML = label;

        this.current = p.get('recordOffset') < 1 ? this.span : this.link;
        return this.current;
    },

    /**
     * Swap the link and span nodes if appropriate.
     * @method update
     * @param e {CustomEvent} The calling change event
     */
    update : function (e) {
        if (e && e.prevValue === e.newValue) {
            return;
        }

        var par = this.current ? this.current.parentNode : null;
        if (this.paginator.get('recordOffset') < 1) {
            if (par && this.current === this.link) {
                par.replaceChild(this.span,this.current);
                this.current = this.span;
            }
        } else {
            if (par && this.current === this.span) {
                par.replaceChild(this.link,this.current);
                this.current = this.link;
            }
        }
    },

    /**
     * Removes the link/span node and clears event listeners
     * @method destroy
     * @private
     */
    destroy : function () {
        YAHOO.util.Event.purgeElement(this.link);
        this.current.parentNode.removeChild(this.current);
        this.link = this.span = null;
    },

    /**
     * Listener for the link's onclick event.  Passes to setPage method.
     * @method onClick
     * @param e {DOMEvent} The click event
     */
    onClick : function (e) {
        YAHOO.util.Event.stopEvent(e);
        this.paginator.setPage(this.paginator.getPreviousPage());
    }
};

})();
(function () {

var Paginator = YAHOO.widget.Paginator,
    l         = YAHOO.lang;

/**
 * ui Component to generate the rows-per-page dropdown
 *
 * @namespace YAHOO.widget.Paginator.ui
 * @class RowsPerPageDropdown
 * @for YAHOO.widget.Paginator
 *
 * @constructor
 * @param p {Pagintor} Paginator instance to attach to
 */
Paginator.ui.RowsPerPageDropdown = function (p) {
    this.paginator = p;

    p.createEvent('rowsPerPageOptionsChange');
    p.createEvent('rowsPerPageDropdownClassChange');

    p.subscribe('rowsPerPageChange',this.update,this,true);
    p.subscribe('rowsPerPageOptionsChange',this.rebuild,this,true);
    p.subscribe('destroy',this.destroy,this,true);

    // TODO: make this work
    p.subscribe('rowsPerPageDropdownClassChange',this.rebuild,this,true);
};

/**
 * Decorates Paginator instances with new attributes. Called during
 * Paginator instantiation.
 * @method init
 * @param p {Paginator} Paginator instance to decorate
 * @static
 */
Paginator.ui.RowsPerPageDropdown.init = function (p) {

    /**
     * Array of available rows-per-page sizes.  Converted into select options.
     * Array values may be positive integers or object literals in the form<br>
     * { value : NUMBER, text : STRING }
     * @attribute rowsPerPageOptions
     * @default []
     */
    p.setAttributeConfig('rowsPerPageOptions', {
        value : [],
        validator : l.isArray
    });

    /**
     * CSS class assigned to the select node
     * @attribute rowsPerPageDropdownClass
     * @default 'yui-pg-rpp-options'
     */
    p.setAttributeConfig('rowsPerPageDropdownClass', {
        value : 'yui-pg-rpp-options',
        validator : l.isString
    });
};

Paginator.ui.RowsPerPageDropdown.prototype = {

    /**
     * select node
     * @property select
     * @type HTMLElement
     * @private
     */
    select  : null,


    /**
     * Generate the select and option nodes and returns the select node.
     * @method render
     * @param id_base {string} used to create unique ids for generated nodes
     * @return {HTMLElement}
     */
    render : function (id_base) {
        this.select = document.createElement('select');
        this.select.id        = id_base + '-rpp';
        this.select.className = this.paginator.get('rowsPerPageDropdownClass');
        this.select.title = 'Rows per page';

        YAHOO.util.Event.on(this.select,'change',this.onChange,this,true);

        this.rebuild();

        return this.select;
    },

    /**
     * Select the appropriate option if changed.
     * @method update
     * @param e {CustomEvent} The calling change event
     */
    update : function (e) {
        if (e && e.prevValue === e.newValue) {
            return;
        }

        var rpp     = this.paginator.get('rowsPerPage'),
            options = this.select.options,
            i,len;

        for (i = 0, len = options.length; i < len; ++i) {
            if (parseInt(options[i].value,10) === rpp) {
                options[i].selected = true;
            }
        }
    },


    /**
     * (Re)generate the select options.
     * @method rebuild
     */
    rebuild : function (e) {
        var p       = this.paginator,
            sel     = this.select,
            options = p.get('rowsPerPageOptions'),
            opt_tem = document.createElement('option'),
            i,len;

        while (sel.firstChild) {
            sel.removeChild(sel.firstChild);
        }

        for (i = 0, len = options.length; i < len; ++i) {
            var node = opt_tem.cloneNode(false),
                opt  = options[i];
            node.value = l.isValue(opt.value) ? opt.value : opt;
            node.innerHTML = l.isValue(opt.text) ? opt.text : opt;
            sel.appendChild(node);
        }

        this.update();
    },

    /**
     * Removes the select node and clears event listeners
     * @method destroy
     * @private
     */
    destroy : function () {
        YAHOO.util.Event.purgeElement(this.select);
        this.select.parentNode.removeChild(this.select);
        this.select = null;
    },

    /**
     * Listener for the select's onchange event.  Sent to setRowsPerPage method.
     * @method onChange
     * @param e {DOMEvent} The change event
     */
    onChange : function (e) {
        this.paginator.setRowsPerPage(
                parseInt(this.select.options[this.select.selectedIndex].value,10));
    }
};

})();
YAHOO.register("paginator", YAHOO.widget.Paginator, {version: "2.6.0", build: "1321"});
